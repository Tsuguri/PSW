#![allow(non_snake_case)]


#[macro_use]
extern crate glium;
extern crate image;
extern crate cgmath;
extern crate rand;
#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_xml_rs;

mod Input;
mod Renderer;
mod Math;
mod Data;

use std::time::Instant;

use Input::KeyboardInput::*;
use Input::MouseInput::*;
use Renderer::DataTypes::*;
use Renderer::Camera::*;
use Math::Vector::Vector3;
use Data::deser;
use glium::framebuffer::ToDepthAttachment;

fn LoadTexture(display: &glium::backend::Facade, fileName: &String) -> glium::texture::Texture2d
{
	use std::path::Path;
	
	let image = image::open(&Path::new(fileName)).unwrap().to_rgba();
	
	let image_dimensions = image.dimensions();
	let image = glium::texture::RawImage2d::from_raw_rgba_reversed(image.into_raw(), image_dimensions);
	glium::texture::Texture2d::new(display,image).unwrap()
}

fn SaveTextureToFile(image: glium::texture::RawImage2d<u8>, name: &str) {
    use std::path::Path;
    let image = image::ImageBuffer::from_raw(image.width, image.height, image.data.into_owned()).unwrap();
    let image = image::DynamicImage::ImageRgba8(image).flipv();
    image.save(name).unwrap();
}

fn LoadFileContent(filename: &String) -> String
{
	use std::fs::File;
	use std::io::prelude::*;
    let mut file = File::open(filename).expect("Unable to open the file");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Unable to read the file");
    contents
}

fn SceneToBsplines(scene: &Data::Scene, display: &glium::backend::Facade) -> Vec<Box<BezierPatch>> {
    let mut v = Vec::with_capacity(200);

    for surf in &scene.surfaces {
        for patch in &surf.patches {
            v.push(PatchToBsplines(patch, display));
        }
    }

    v
}

fn LineFromPoints<'a, I>(points: I, display: &glium::backend::Facade) -> Model<PositionVertex> 
    where I: Iterator<Item=&'a Vector3<f32>>
{
    let mut verts = Vec::new();
    for i in points {
        //println!("{:?}", *i);
        verts.push(PositionVertex::new(*i));
    }
    let indices: Vec<u32> = (0..verts.len() as u32).collect();

    let v = glium::VertexBuffer::new(display, &verts).unwrap();
    let i = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::Points, &indices).unwrap();
    Model::new(v,i)

}

fn rect(display: &glium::backend::Facade) -> Model<PositionVertex> {
   let verts = vec![
    PositionVertex::new(Vector3::new(-1.0,-1.0,0.0)),
    PositionVertex::new(Vector3::new(-1.0,1.0,0.0)),
    PositionVertex::new(Vector3::new(1.0,-1.0,0.0)),
    PositionVertex::new(Vector3::new(1.0,1.0,0.0)),
   ]; 
   let indices = vec![0,1,2, 2,1,3];
   let v = glium::VertexBuffer::new(display, &verts).unwrap();
   let i = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::TrianglesList, &indices).unwrap();
   Model::new(v,i)
}

fn PatchToBsplines(patch: &Data::Patch, display: &glium::backend::Facade) -> Box<BezierPatch> {

    let shape = [
        PositionVertex::new(patch.points[0][0]),
        PositionVertex::new(patch.points[0][1]),
        PositionVertex::new(patch.points[0][2]),
        PositionVertex::new(patch.points[0][3]),
        PositionVertex::new(patch.points[1][0]),
        PositionVertex::new(patch.points[1][1]),
        PositionVertex::new(patch.points[1][2]),
        PositionVertex::new(patch.points[1][3]),
        PositionVertex::new(patch.points[2][0]),
        PositionVertex::new(patch.points[2][1]),
        PositionVertex::new(patch.points[2][2]),
        PositionVertex::new(patch.points[2][3]),
        PositionVertex::new(patch.points[3][0]),
        PositionVertex::new(patch.points[3][1]),
        PositionVertex::new(patch.points[3][2]),
        PositionVertex::new(patch.points[3][3]),
    ];
	let verts = glium::VertexBuffer::new(display, &shape).unwrap();
	let verts2 = glium::VertexBuffer::new(display, &shape).unwrap();
	let indic = vec![0u32, 1, 2, 3, 7, 6, 5, 4, 8, 9, 10, 11, 15, 14,  13, 12, 8, 4, 0, 1, 5, 9, 13, 14, 10, 6, 2, 3, 7, 11, 15];
	let indic2 = vec![0u32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
	let indices = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::LineStrip, &indic).unwrap();
	let surfIndic = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::Patches{ vertices_per_patch : 16}, &indic2).unwrap();
	Box::new(BezierPatch {mesh : Model::new(verts, indices), surface : Model::new(verts2, surfIndic), x : 0, y : 0})
}


struct BezierPatch
{
	pub x : i32,
	pub y : i32,
	pub mesh : Model<PositionVertex>,
	pub surface : Model<PositionVertex>
}

fn GetWingsLines(surf: &Data::Surface, display: &glium::backend::Facade) -> Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let u = surf.u as f32;
    // prawa krawedz
    let poss : Vec<Vector3<f32>> = (0..100).map(|x| surf.evaluate(u*(x as f32)/100.0, surf.v as f32 - 0.0001)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    // uszczypniecie
    // troche za długie, ale nie ma co się bawić
    let poss : Vec<Vector3<f32>> = (65..75).map(|x| surf.evaluate(u*(x as f32)/100.0, u)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    // lewa krawedz
    let poss : Vec<Vector3<f32>> = (0..100).map(|x| surf.evaluate(u*(x as f32)/100.0, 0.0)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));


    lines
}

fn GetGatlingLines(surf: &Data::Surface, display: &glium::backend::Facade)->Vec<Model<PositionVertex>> {

    let mut lines = Vec::new();
    let u = surf.u as f32;

    // prawa krawedz
    let poss : Vec<Vector3<f32>> = (0..5).map(|x| surf.evaluate(u*(x as f32)/5.0, 0.5)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn GetCockpitLines(surf: &Data::Surface, display: &glium::backend::Facade)->Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let v = surf.v as f32;

    let poss : Vec<Vector3<f32>> = (70..100).map(|x| surf.evaluate(surf.u as f32 - 1.0, v*(x as f32)/100.0)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn GetBodyLines(surf: &Data::Surface, display: &glium::backend::Facade)->Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let u = surf.u as f32;

    // prawa krawedz
    let poss : Vec<Vector3<f32>> = (0..100).map(|x| surf.evaluate(u*(x as f32)/100.0, 3.0)).collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn render<S>(target: &mut S, d: &RenderingData) where S: glium::Surface {
	let lightPos = Vector3::<f32>::new(d.t.sin(), 2.0, d.t.cos());

	target.clear_color(0.0,0.6,0.7,1.0);
	target.clear_depth(1.0);
	
	let viewMat = d.camera.GetViewMatrix().Transposed();
	let perspectiveMat = d.camera.GetProjectionMatrix().Transposed();
	let mat = viewMat * perspectiveMat;

	for bspl in &d.surfaces
	{
	    target.draw(bspl.surface.GetVertices(),bspl.surface.GetIndices(), &d.bsplineProgram, &uniform!{ t: d.t, 
		    lightPosition : lightPos.Content(), 
		    cameraPosition : d.camera.GetPosition().Content(), 
		    inner: 1.0f32, 
		    outer: 1.0f32, 
		    texDiffuse: &d.diffuseTexture, 
		    //texNormal: &normalTexture, 
		    //texHeight: &heightTexture, 
		    viewModel: viewMat.Content(), 
		    perspective : perspectiveMat.Content(),
            tool: d.toolRadius,
		    x: bspl.x,
		    y: bspl.y
        }, &d.surfaceParams).unwrap();
		
	}
    for line in &d.lines {
        //println!("drawing line!");
        target.draw(line.GetVertices(), line.GetIndices(), &d.contLinesProgram , &uniform!{t:d.t, mat: mat.Content(), tool: d.toolRadius}, &d.surfaceParams).unwrap();
    }
}

struct RenderingData<'a> {
    diffuseTexture:  glium::texture::Texture2d,
    camera: Camera,
    surfaces: Vec<Box<BezierPatch>>,
    lines: Vec<Model<PositionVertex>>,
    surfaceParams: glium::DrawParameters<'a>,
    t: f32,
    toolRadius: f32,
    bsplineProgram: glium::Program,
    contLinesProgram: glium::Program,

}

fn main()
{
    let mut lines = Vec::<Model<PositionVertex>>::new();

	use glium::{DisplayBuild, Surface};
	let display = glium::glutin::WindowBuilder::new().with_title("Super Paths").build_glium().unwrap();

	let diffuseTexture = LoadTexture(&display, &String::from("res/diffuse.jpg"));

	let triangleVert = LoadFileContent(&String::from("res/Shaders/triangleVert.vert"));
	let outlineVert = LoadFileContent(&String::from("res/Shaders/outlineVert.vert"));
	let triangleFrag = LoadFileContent(&String::from("res/Shaders/triangleFrag.frag"));
	let outlineFrag = LoadFileContent(&String::from("res/Shaders/outlineFrag.frag"));
	let triangleTessControl = LoadFileContent(&String::from("res/Shaders/triangleTessControl.tesc"));
	let bsplineTessControl = LoadFileContent(&String::from("res/Shaders/bsplineTessControl.tesc"));
	let triangleTessEval = LoadFileContent(&String::from("res/Shaders/triangleTessEval.tese"));
	let bsplineTessEval = LoadFileContent(&String::from("res/Shaders/bsplineTessEval.tese"));
    let spheresGeometry = LoadFileContent(&String::from("res/Shaders/spheresGeometry.geom"));

    let rectVert = LoadFileContent(&String::from("res/Shaders/rect.vert"));
    let rectFrag = LoadFileContent(&String::from("res/Shaders/rect.frag"));

    let sceneData = deser("res/model.xml");

    let bspline = SceneToBsplines(&sceneData, &display);
    let rect = rect(&display);

    // 0 - gatling
    // 1 - kadlub
    // 2 - kokpit
    // 3 - skrzydla


    // gatling - czubek
    // skrzydla - brzegi
    // skrzydlo prawe - uszczypniecie
    // kokpit - ster
    // kadlub - tyl - na plasko
    // kadlub - dziub - dookola
    //
    
    lines.append(&mut GetWingsLines(&sceneData.surfaces[3], &display));
    lines.append(&mut GetCockpitLines(&sceneData.surfaces[2], &display));
    lines.append(&mut GetGatlingLines(&sceneData.surfaces[0], &display));
    lines.append(&mut GetBodyLines(&sceneData.surfaces[1], &display));

    // razem jest 6 nieciągłości do rozważenia


	let mut keyboard : KeyboardState = Default::default();
	let mut mouse : MouseState = Default::default();


    let spheresShaderSource = glium::program::SourceCode {
        vertex_shader: &outlineVert,
        tessellation_control_shader: Option::None,
        tessellation_evaluation_shader: Option::None,
        geometry_shader: Option::Some(&spheresGeometry),
        fragment_shader: &outlineFrag
    };
	//let shaderProgram = glium::Program::new(&display, shaderSource).unwrap();
    let contLinesProgram = glium::Program::new(&display, spheresShaderSource).unwrap();

    let rectProgram = glium::Program::from_source(&display, &rectVert, &rectFrag, Option::None).unwrap();

	let shaderSource = glium::program::SourceCode { vertex_shader : &triangleVert,
									 tessellation_control_shader : Option::Some(&bsplineTessControl),
									 tessellation_evaluation_shader : Option::Some(&bsplineTessEval),
									 geometry_shader : Option::None,
									 fragment_shader : &triangleFrag };
	let bsplineProgram = glium::Program::new(&display, shaderSource).unwrap();

	/*let outlineParams = glium::DrawParameters {
		polygon_mode : glium::draw_parameters::PolygonMode::Line,
		depth : glium::Depth {test : glium::DepthTest::IfLess, write : true, ..Default::default()},
		.. Default::default()
	};*/
	let mut surfaceParams = glium::DrawParameters {
		polygon_mode: glium::draw_parameters::PolygonMode::Fill,
		depth : glium::Depth {test : glium::DepthTest::IfLess, write : true, ..Default::default()},		
		.. Default::default()
	};

    let depth = glium::texture::Texture2d::empty(&display, 1920, 1080).unwrap();
    let depthBuf = glium::texture::depth_texture2d::DepthTexture2d::empty(&display, 1920, 1080).unwrap();

    let mut fb = glium::framebuffer::SimpleFrameBuffer::with_depth_buffer(&display, &depth, &depthBuf).unwrap();
    let mut fb2 = glium::framebuffer::SimpleFrameBuffer::new(&display, &depth).unwrap();

    display.release_shader_compiler();

    let mut drawParams = RenderingData {
        surfaceParams,
	    camera :Camera::new(-45.0, 30.0, Vector3::new(0.0, 0.0, 0.0), 3.0,  3.14/5.0, 0.1, 50.0, 3.0/4.0),
        diffuseTexture,
        lines,
        surfaces: bspline,
        t: -0.5,
        toolRadius: -0.8,
        bsplineProgram,
        contLinesProgram
    };

	let mut now = Instant::now();
	let mut filled : bool = true;
	loop
	{
		if(&keyboard).IsButtonDown(KeyCode::Escape)
		{
			return;
		}
		let dt = now.elapsed();
		let duration = dt.as_secs() as f32 + dt.subsec_nanos() as f32 / 1_000_000_000.0;
		now = Instant::now();
		drawParams.t += duration;

		let mouseDT = mouse.GetMouseMove();
		if mouse.RightButtonDown()
		{
			drawParams.camera.ModifyRadius(mouseDT[1] / 10.0);
		}
		if mouse.LeftButtonDown()
		{
			drawParams.camera.ModifyXAngle(-mouseDT[1] / 3.0);
			drawParams.camera.ModifyYAngle(-mouseDT[0] / 3.0);
			
		}
		if keyboard.ButtonPressedInLastFrame(KeyCode::F)
		{
			filled = !filled;
			if filled
			{
				surfaceParams.polygon_mode = glium::draw_parameters::PolygonMode::Fill;
			}
			else
			{
				surfaceParams.polygon_mode = glium::draw_parameters::PolygonMode::Line;				
			}
		}
        if keyboard.ButtonPressedInLastFrame(KeyCode::P) {
           //SaveTextureToFile(display.read_front_buffer(), "/home/adam/tmp.png"); 

           println!("rendering");
           render(&mut fb, &drawParams);

            fb2.draw(rect.GetVertices(), rect.GetIndices(), &rectProgram, &uniform!(texSampler: depthBuf.sampled()), &glium::DrawParameters{..Default::default()}).unwrap();

           //SaveTextureToFile(fb, "/home/adam/tmp.png"); 
           //let pixels : Vec<(u8, u8, u8, u8)> = depth.read();
           //let data :i32 = pixels.read().unwrap();

           println!("saving");
           SaveTextureToFile(depth.read(), "/home/adam/depth.png");
           println!("done");
        }
		let mut target = display.draw();

        render(&mut target, &drawParams);



		target.finish().unwrap();

		mouse.mouseMoved = false;
		keyboard.NextFrame();
		for ev in display.poll_events()
		{
			match ev {
				glium::glutin::Event::Closed => return,
				glium::glutin::Event::KeyboardInput(state,_,elem)=>{
					let val: bool = match state
					{
						glium::glutin::ElementState::Pressed => true,
						glium::glutin::ElementState::Released => false
					};
					match elem{
						Some(keyCode) => keyboard.SetButton(KeyCode::FromGlutinEnum(keyCode),val),
						_=>()
					}
				}
				glium::glutin::Event::MouseMoved(X, Y) =>{
					mouse.SetNewPosition([X as f32,Y as f32]);
				}
				glium::glutin::Event::MouseInput(state, elem) =>{
					let val: bool = match state
					{
						glium::glutin::ElementState::Pressed => true,
						glium::glutin::ElementState::Released => false
					};
					match elem{
						glium::glutin::MouseButton::Left => mouse.SetButtonState(0, val),
						glium::glutin::MouseButton::Right => mouse.SetButtonState(1, val),
						_=>()
					}
				}
				_=> ()
			}
		}
	}
}
