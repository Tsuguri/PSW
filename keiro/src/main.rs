#![allow(non_snake_case)]

#[macro_use]
extern crate glium;
extern crate cgmath;
extern crate image;
extern crate rand;
#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_xml_rs;

mod Data;
mod Input;
mod Math;
mod Paths;
mod Renderer;
mod Utils;

use std::time::Instant;

use Data::deser;
use Input::KeyboardInput::*;
use Input::MouseInput::*;
use Math::Matrix::Matrix4;
use Math::Vector::Vector3;
use Renderer::Camera::*;
use Renderer::DataTypes::*;
//use glium::framebuffer::ToDepthAttachment;
use Utils::{LoadFileContent, LoadTexture, SaveTextureToFile};

use Paths::{generate_contour, generate_details, generate_flat, generate_rough};

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
where
    I: Iterator<Item = &'a Vector3<f32>>,
{
    let mut verts = Vec::new();
    for i in points {
        //println!("{:?}", *i);
        verts.push(PositionVertex::new(*i));
    }
    let indices: Vec<u32> = (0..verts.len() as u32).collect();

    let v = glium::VertexBuffer::new(display, &verts).unwrap();
    let i = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::Points, &indices)
        .unwrap();
    Model::new(v, i)
}

fn groundRect(display: &glium::backend::Facade, side: f32, height: f32) -> Model<PositionVertex> {
    let verts = vec![
        PositionVertex::new(Vector3::new(-side, height, -side)),
        PositionVertex::new(Vector3::new(-side, height, side)),
        PositionVertex::new(Vector3::new(side, height, -side)),
        PositionVertex::new(Vector3::new(side, height, side)),
    ];
    let indices = vec![0, 1, 2, 2, 1, 3];
    let v = glium::VertexBuffer::new(display, &verts).unwrap();
    let i = glium::index::IndexBuffer::new(
        display,
        glium::index::PrimitiveType::TrianglesList,
        &indices,
    ).unwrap();
    Model::new(v, i)
}
fn rect(display: &glium::backend::Facade, side: f32) -> Model<PositionVertex> {
    let verts = vec![
        PositionVertex::new(Vector3::new(-side, -side, 0.0)),
        PositionVertex::new(Vector3::new(-side, side, 0.0)),
        PositionVertex::new(Vector3::new(side, -side, 0.0)),
        PositionVertex::new(Vector3::new(side, side, 0.0)),
    ];
    let indices = vec![0, 1, 2, 2, 1, 3];
    let v = glium::VertexBuffer::new(display, &verts).unwrap();
    let i = glium::index::IndexBuffer::new(
        display,
        glium::index::PrimitiveType::TrianglesList,
        &indices,
    ).unwrap();
    Model::new(v, i)
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
    let indic = vec![
        0u32, 1, 2, 3, 7, 6, 5, 4, 8, 9, 10, 11, 15, 14, 13, 12, 8, 4, 0, 1, 5, 9, 13, 14, 10, 6,
        2, 3, 7, 11, 15,
    ];
    let indic2 = vec![0u32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    let indices =
        glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::LineStrip, &indic)
            .unwrap();
    let surfIndic = glium::index::IndexBuffer::new(
        display,
        glium::index::PrimitiveType::Patches {
            vertices_per_patch: 16,
        },
        &indic2,
    ).unwrap();
    Box::new(BezierPatch {
        mesh: Model::new(verts, indices),
        surface: Model::new(verts2, surfIndic),
        x: 0,
        y: 0,
    })
}

struct BezierPatch {
    pub x: i32,
    pub y: i32,
    pub mesh: Model<PositionVertex>,
    pub surface: Model<PositionVertex>,
}

fn GetWingsLines(
    surf: &Data::Surface,
    display: &glium::backend::Facade,
) -> Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let u = surf.u as f32;
    // prawa krawedz
    let poss: Vec<Vector3<f32>> = (0..100)
        .map(|x| surf.evaluate(u * (x as f32) / 100.0, surf.v as f32 - 0.0001))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    // uszczypniecie
    // troche za długie, ale nie ma co się bawić
    let poss: Vec<Vector3<f32>> = (65..75)
        .map(|x| surf.evaluate(u * (x as f32) / 100.0, u))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    // lewa krawedz
    let poss: Vec<Vector3<f32>> = (0..100)
        .map(|x| surf.evaluate(u * (x as f32) / 100.0, 0.0))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn GetGatlingLines(
    surf: &Data::Surface,
    display: &glium::backend::Facade,
) -> Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let u = surf.u as f32;

    // prawa krawedz
    let poss: Vec<Vector3<f32>> = (0..5)
        .map(|x| surf.evaluate(u * (x as f32) / 5.0, 0.5))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn GetCockpitLines(
    surf: &Data::Surface,
    display: &glium::backend::Facade,
) -> Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let v = surf.v as f32;

    let poss: Vec<Vector3<f32>> = (70..100)
        .map(|x| surf.evaluate(surf.u as f32 - 1.0, v * (x as f32) / 100.0))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn GetBodyLines(
    surf: &Data::Surface,
    display: &glium::backend::Facade,
) -> Vec<Model<PositionVertex>> {
    let mut lines = Vec::new();
    let u = surf.u as f32;

    // prawa krawedz
    let poss: Vec<Vector3<f32>> = (0..100)
        .map(|x| surf.evaluate(u * (x as f32) / 100.0, 3.0))
        .collect();
    lines.push(LineFromPoints((&poss).into_iter(), display));

    lines
}

fn render<S>(
    target: &mut S,
    d: &RenderingData,
    viewMat: &Matrix4<f32>,
    perspectiveMat: &Matrix4<f32>,
) where
    S: glium::Surface,
{
    let lightPos = Vector3::<f32>::new(d.t.sin(), 2.0, d.t.cos());

    target.clear_color(0.0, 0.6, 0.7, 1.0);
    target.clear_depth(1.0);

    let mat = *viewMat * *perspectiveMat;

    for bspl in &d.surfaces {
        target
            .draw(
                bspl.surface.GetVertices(),
                bspl.surface.GetIndices(),
                &d.bsplineProgram,
                &uniform!{ t: d.t,
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
                },
                &d.surfaceParams,
            ).unwrap();
    }
    for line in &d.lines {
        //println!("drawing line!");
        target
            .draw(
                line.GetVertices(),
                line.GetIndices(),
                &d.contLinesProgram,
                &uniform!{t:d.t, mat: mat.Content(), tool: d.toolRadius},
                &d.surfaceParams,
            ).unwrap();
    }

    target
        .draw(
            d.ground.GetVertices(),
            d.ground.GetIndices(),
            &d.groundProgram,
            &uniform!{mat:mat.Content()},
            &d.surfaceParams,
        ).unwrap();

    match &d.l2 {
        None => (),
        Some(l) => {
            target
                .draw(
                    l.GetVertices(),
                    l.GetIndices(),
                    &d.contLinesProgram,
                    &uniform!{mat:mat.Content(), t:d.t, tool: 0.05f32},
                    &d.surfaceParams,
                ).unwrap();
        }
    }
}

fn points_to_file(pts: &Vec<Vector3<f32>>, path: &str) {
    use std::fs::File;
    use std::io::Write;
    // N3G01X00.000Y00.000Z60.000
    //
    let mut file = File::create(path).unwrap();
    for pt in pts.iter().enumerate() {
        write!(
            &mut file,
            "N{}G01X{:.3}Y{:.3}Z{:.3}\n",
            pt.0 + 1,
            pt.1.x() * 10.0,
            pt.1.y() * 10.0,
            pt.1.z() * 10.0
        );
    }

    drop(file);
}

struct RenderingData<'a> {
    diffuseTexture: glium::texture::Texture2d,
    camera: Camera,
    surfaces: Vec<Box<BezierPatch>>,
    lines: Vec<Model<PositionVertex>>,
    l2: Option<Model<PositionVertex>>,
    ground: Model<PositionVertex>,
    groundProgram: glium::Program,
    surfaceParams: glium::DrawParameters<'a>,
    t: f32,
    toolRadius: f32,
    bsplineProgram: glium::Program,
    contLinesProgram: glium::Program,
}

fn main() {
    let mut lines = Vec::<Model<PositionVertex>>::new();

    use glium::{DisplayBuild, Surface};
    let display = glium::glutin::WindowBuilder::new()
        .with_title("Super Paths")
        .build_glium()
        .unwrap();

    let diffuseTexture = LoadTexture(&display, &String::from("res/diffuse.jpg"));

    let triangleVert = LoadFileContent(&String::from("res/Shaders/triangleVert.vert"));
    let outlineVert = LoadFileContent(&String::from("res/Shaders/outlineVert.vert"));
    let triangleFrag = LoadFileContent(&String::from("res/Shaders/triangleFrag.frag"));
    let outlineFrag = LoadFileContent(&String::from("res/Shaders/outlineFrag.frag"));
    //let triangleTessControl = LoadFileContent(&String::from("res/Shaders/triangleTessControl.tesc"));
    let bsplineTessControl = LoadFileContent(&String::from("res/Shaders/bsplineTessControl.tesc"));
    //let triangleTessEval = LoadFileContent(&String::from("res/Shaders/triangleTessEval.tese"));
    let bsplineTessEval = LoadFileContent(&String::from("res/Shaders/bsplineTessEval.tese"));
    let spheresGeometry = LoadFileContent(&String::from("res/Shaders/spheresGeometry.geom"));

    let rectVert = LoadFileContent(&String::from("res/Shaders/rect.vert"));
    let rectFrag = LoadFileContent(&String::from("res/Shaders/rect.frag"));
    let groundVert = LoadFileContent(&String::from("res/Shaders/ground.vert"));
    let groundFrag = LoadFileContent(&String::from("res/Shaders/ground.frag"));

    let sceneData = deser("res/model2.xml");

    let bspline = SceneToBsplines(&sceneData, &display);
    let rect2 = rect(&display, 1.0f32);

    let ground = groundRect(&display, 7.9f32, 0.9);

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

    let mut keyboard: KeyboardState = Default::default();
    let mut mouse: MouseState = Default::default();

    let spheresShaderSource = glium::program::SourceCode {
        vertex_shader: &outlineVert,
        tessellation_control_shader: Option::None,
        tessellation_evaluation_shader: Option::None,
        geometry_shader: Option::Some(&spheresGeometry),
        fragment_shader: &outlineFrag,
    };
    //let shaderProgram = glium::Program::new(&display, shaderSource).unwrap();
    let contLinesProgram = glium::Program::new(&display, spheresShaderSource).unwrap();

    let rectProgram =
        glium::Program::from_source(&display, &rectVert, &rectFrag, Option::None).unwrap();
    let groundProgram =
        glium::Program::from_source(&display, &groundVert, &groundFrag, Option::None).unwrap();

    let shaderSource = glium::program::SourceCode {
        vertex_shader: &triangleVert,
        tessellation_control_shader: Option::Some(&bsplineTessControl),
        tessellation_evaluation_shader: Option::Some(&bsplineTessEval),
        geometry_shader: Option::None,
        fragment_shader: &triangleFrag,
    };
    let bsplineProgram = glium::Program::new(&display, shaderSource).unwrap();

    /*let outlineParams = glium::DrawParameters {
		polygon_mode : glium::draw_parameters::PolygonMode::Line,
		depth : glium::Depth {test : glium::DepthTest::IfLess, write : true, ..Default::default()},
		.. Default::default()
	};*/
    let mut surfaceParams = glium::DrawParameters {
        polygon_mode: glium::draw_parameters::PolygonMode::Fill,
        depth: glium::Depth {
            test: glium::DepthTest::IfLess,
            write: true,
            ..Default::default()
        },
        ..Default::default()
    };

    let depSize = 1872;
    let depth = glium::texture::Texture2d::empty(&display, depSize, depSize).unwrap();
    let depthBuf =
        glium::texture::depth_texture2d::DepthTexture2d::empty(&display, depSize, depSize).unwrap();

    let mut fb =
        glium::framebuffer::SimpleFrameBuffer::with_depth_buffer(&display, &depth, &depthBuf)
            .unwrap();
    let mut fb2 = glium::framebuffer::SimpleFrameBuffer::new(&display, &depth).unwrap();

    display.release_shader_compiler();

    let mut drawParams = RenderingData {
        surfaceParams,
        camera: Camera::new(
            -45.0,
            30.0,
            Vector3::new(0.0, 0.0, 0.0),
            3.0,
            3.14 / 5.0,
            0.1,
            50.0,
            3.0 / 4.0,
        ),
        diffuseTexture,
        lines,
        l2: Option::None,
        ground,
        groundProgram,
        surfaces: bspline,
        t: -0.5,
        toolRadius: -0.9,
        bsplineProgram,
        contLinesProgram,
    };

    let mut now = Instant::now();
    let mut filled: bool = true;
    loop {
        if (&keyboard).IsButtonDown(KeyCode::Escape) {
            return;
        }
        let dt = now.elapsed();
        let duration = dt.as_secs() as f32 + dt.subsec_nanos() as f32 / 1_000_000_000.0;
        now = Instant::now();
        drawParams.t += duration;

        let mouseDT = mouse.GetMouseMove();
        if mouse.RightButtonDown() {
            drawParams.camera.ModifyRadius(mouseDT[1] / 10.0);
        }
        if mouse.LeftButtonDown() {
            drawParams.camera.ModifyXAngle(-mouseDT[1] / 3.0);
            drawParams.camera.ModifyYAngle(-mouseDT[0] / 3.0);
        }
        if keyboard.ButtonPressedInLastFrame(KeyCode::F) {
            filled = !filled;
            if filled {
                surfaceParams.polygon_mode = glium::draw_parameters::PolygonMode::Fill;
            } else {
                surfaceParams.polygon_mode = glium::draw_parameters::PolygonMode::Line;
            }
        }
        if keyboard.ButtonPressedInLastFrame(KeyCode::P) {
            //SaveTextureToFile(display.read_front_buffer(), "/home/adam/tmp.png");

            println!("rendering");

            let viewMat = Matrix4::Translation(0.0, 0.0, 5.0) * Matrix4::RotationX(-3.14 / 2.0);
            let viewMat = viewMat.Transposed();
            //let perspectiveMat = drawParams.camera.GetProjectionMatrix().Transposed();
            let perspectiveMat = Matrix4::<f32>::Ortho(-7.5f32, 7.5, -7.5, 7.5).Transposed();
            drawParams.toolRadius = -0.9;
            drawParams.ground = groundRect(&display, 7.9f32, 0.9);
            render(&mut fb, &drawParams, &viewMat, &perspectiveMat);

            fb2.draw(
                rect2.GetVertices(),
                rect2.GetIndices(),
                &rectProgram,
                &uniform!(texSampler: depthBuf.sampled()),
                &glium::DrawParameters {
                    ..Default::default()
                },
            ).unwrap();

            //SaveTextureToFile(fb, "/home/adam/tmp.png");
            //let pixels : Vec<(u8, u8, u8, u8)> = depth.read();
            //let data :i32 = pixels.read().unwrap();
            println!("reading data");
            let data = depth.read();

            println!("genrating rough paths");
            let roughPath = generate_rough(
                -7.5,
                7.5,
                -7.5,
                7.5,
                2.0,
                5.0,
                0.8,
                (-10.0, -10.0, 5.0),
                data,
            );

            points_to_file(&roughPath, "/home/adam/paths/r1.k16");

            let ground2 = groundRect(&display, 10.0f32, 0.0);
            drawParams.ground = ground2;
            drawParams.toolRadius = -0.6;

            let viewMat = Matrix4::Translation(0.0, 0.0, 5.0) * Matrix4::RotationX(-3.14 / 2.0);
            let viewMat = viewMat.Transposed();
            //let perspectiveMat = drawParams.camera.GetProjectionMatrix().Transposed();
            let side = 7.8;
            let perspectiveMat = Matrix4::<f32>::Ortho(-side, side, -side, side).Transposed();
            render(&mut fb, &drawParams, &viewMat, &perspectiveMat);

            fb2.draw(
                rect2.GetVertices(),
                rect2.GetIndices(),
                &rectProgram,
                &uniform!(texSampler: depthBuf.sampled()),
                &glium::DrawParameters {
                    ..Default::default()
                },
            ).unwrap();

            let data = depth.read();

            let flat = generate_flat(
                -7.5,
                7.5,
                -7.5,
                7.5,
                2.0,
                5.0,
                0.6,
                (-10.0, -10.0, 5.0),
                data,
            );

            points_to_file(&flat, "/home/adam/paths/r2.f12");

            drawParams.toolRadius = -0.5;
            render(&mut fb, &drawParams, &viewMat, &perspectiveMat);
            fb2.draw(
                rect2.GetVertices(),
                rect2.GetIndices(),
                &rectProgram,
                &uniform!(texSampler: depthBuf.sampled()),
                &glium::DrawParameters {
                    ..Default::default()
                },
            ).unwrap();
            let flat_contour = generate_contour(
                -7.5,
                7.5,
                -7.5,
                7.5,
                2.0,
                5.0,
                0.5,
                (-10.0, -10.0, 5.0),
                depth.read(),
            );

            //drawParams.l2 = Option::Some(LineFromPoints( flat_contour.iter(), &display));
            points_to_file(&flat_contour, "/home/adam/paths/r3.f10");

            /*
            drawParams.toolRadius = -0.4;
            drawParams.ground = groundRect(&display, 7.9f32, -drawParams.toolRadius);

            render(&mut fb, &drawParams, &viewMat, &perspectiveMat);
            fb2.draw(
                rect2.GetVertices(),
                rect2.GetIndices(),
                &rectProgram,
                &uniform!(texSampler: depthBuf.sampled()),
                &glium::DrawParameters {
                    ..Default::default()
                },
            ).unwrap();
            let details = generate_details(
                -7.5,
                7.5,
                -7.5,
                7.5,
                2.0,
                5.0,
                -drawParams.toolRadius,
                (-10.0, -10.0, 5.0),
                depth.read(),
                &sceneData,
            );

            points_to_file(&details, "/home/adam/paths/r4.k8");
            println!("generated {} points", details.len());
            drawParams.l2 = Option::Some(LineFromPoints(details.iter(), &display));
            */
            //SaveTextureToFile(depth.read(), "/home/adam/depth.png");

            //println!("saving");
            println!("done");
        }
        let mut target = display.draw();

        let viewMat = drawParams.camera.GetViewMatrix().Transposed();
        let perspectiveMat = drawParams.camera.GetProjectionMatrix().Transposed();
        render(&mut target, &drawParams, &viewMat, &perspectiveMat);

        target.finish().unwrap();

        mouse.mouseMoved = false;
        keyboard.NextFrame();
        for ev in display.poll_events() {
            match ev {
                glium::glutin::Event::Closed => return,
                glium::glutin::Event::KeyboardInput(state, _, elem) => {
                    let val: bool = match state {
                        glium::glutin::ElementState::Pressed => true,
                        glium::glutin::ElementState::Released => false,
                    };
                    match elem {
                        Some(keyCode) => keyboard.SetButton(KeyCode::FromGlutinEnum(keyCode), val),
                        _ => (),
                    }
                }
                glium::glutin::Event::MouseMoved(X, Y) => {
                    mouse.SetNewPosition([X as f32, Y as f32]);
                }
                glium::glutin::Event::MouseInput(state, elem) => {
                    let val: bool = match state {
                        glium::glutin::ElementState::Pressed => true,
                        glium::glutin::ElementState::Released => false,
                    };
                    match elem {
                        glium::glutin::MouseButton::Left => mouse.SetButtonState(0, val),
                        glium::glutin::MouseButton::Right => mouse.SetButtonState(1, val),
                        _ => (),
                    }
                }
                _ => (),
            }
        }
    }
}
