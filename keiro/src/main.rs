#![allow(non_snake_case)]


#[macro_use]
extern crate glium;
extern crate image;
extern crate cgmath;

#[macro_use]
extern crate serde;
#[macro_use]
extern crate serde_derive;
extern crate serde_xml_rs;

mod Input;
mod Renderer;
mod Math;
mod Data;

use std::time::Instant;

use Input::KeyboardInput::*;
use Renderer::DataTypes::*;
use Renderer::Camera::*;
use Math::Matrix::Matrix4;
use cgmath::Vector3;


fn GenrateVoxelPoints(display: &glium::backend::Facade, size: usize) -> Box<Model>
{
	let mut indic : Vec<u32> = Vec::with_capacity(size*size*size);
	for i in 0..size*size*size
	{
		indic.push(i as u32);
	}
	let indices = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::Points, &indic).unwrap();

	let mut points : Vec<Vertex> = Vec::with_capacity(size*size*size);

	for i in 0..size
	{
		for j in 0..size
		{
			for k in 0..size
			{
				points.push(Vertex { position: [i as f32, j as f32, k as f32], normal: [0.0, 0.0, 0.0], tex: [0.0, 0.0]});
			}
		}
	}
	let verts = glium::VertexBuffer::new(display,&points).unwrap();
	
	Box::new(Model::new(verts, indices))
}

fn GenerateCube(display: &glium::backend::Facade) -> Box<Model>
{
	let shape = vec![
	Vertex { position: [-0.5, -0.5, 0.0], normal: [0.0, 0.0, 0.0], tex: [0.0, 0.0]},
	Vertex { position: [0.0, 0.5, 0.0], normal: [0.0, 0.0, 0.0], tex: [0.0, 1.0]},
	Vertex { position: [0.5, -0.25, 0.0], normal: [0.0, 0.0, 0.0], tex: [1.0, 1.0]},
	Vertex { position: [0.0, -0.5, 0.0], normal: [0.0, 0.0, 0.0], tex: [1.0, 1.0]},
			];
	let verts = glium::VertexBuffer::new(display,&shape).unwrap();

	let indic = vec![
		0u32,1,2,
		0,2,3
	];
	let indices = glium::index::IndexBuffer::new(display, glium::index::PrimitiveType::TrianglesList, &indic).unwrap();

	Box::new(Model::new(verts, indices))
}

fn LoadTexture(display: &glium::backend::Facade, fileName: &String) -> glium::texture::Texture2d
{
	use std::path::Path;
	
	let image = image::open(&Path::new(fileName)).unwrap().to_rgba();
	
	let image_dimensions = image.dimensions();
	let image = glium::texture::RawImage2d::from_raw_rgba_reversed(image.into_raw(), image_dimensions);
	glium::texture::Texture2d::new(display,image).unwrap()
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

fn main()
{
	use glium::{DisplayBuild, Surface};
	let display = glium::glutin::WindowBuilder::new().build_glium().unwrap();

	let texture = LoadTexture(&display, &String::from("res/purple.jpg"));
	let triangleVert = LoadFileContent(&String::from("res/Shaders/triangleVert.glsl"));
	let triangleFrag = LoadFileContent(&String::from("res/Shaders/triangleFrag.glsl"));
    let scene = Data::deser(&"res/model.xml");
	let cube = GenerateCube(&display);
	let voxels = GenrateVoxelPoints(&display, 10);
	let mut camera = Camera::new(45.0, 30.0, Vector3::new(0.0, 0.0, 0.0), 3.0,  3.14/5.0, 0.1, 50.0, 3.0/4.0);

	let mut keyboard : KeyboardState = Default::default();

	let matrix = Matrix4::new(
    0.1, 0.0, 0.0, 0.0,
    0.0, 0.1, 0.0, 0.0,
	0.0, 0.0, 0.1, 0.0,
    0.0, 0.0, 0.0, 1.0);

	let shaderProgram = glium::Program::from_source(&display, &triangleVert, &triangleFrag, None).unwrap();

	let params = glium::DrawParameters {
		point_size: Option::Some(4.0),
		.. Default::default()
	};

	let mut t: f32 = -0.5;

	let mut now = Instant::now();
	loop
	{
		if(&keyboard).IsButtonDown(KeyCode::Escape)
		{
			return;
		}
		let dt = now.elapsed();
		let duration = dt.as_secs() as f32 + dt.subsec_nanos() as f32 / 1_000_000_000.0;
		now = Instant::now();
		t += duration;
		if t>4.0
		{
			t=-4.0;
		}
		camera.SetYAngle(t.cos()*45.0);

		let mut target = display.draw();
		target.clear_color(0.0,0.6,0.7,1.0);

		let mat = (camera.GetProjectionMatrix() * camera.GetViewMatrix() * matrix).Transposed();
		target.draw(cube.GetVertices(), cube.GetIndices(), &shaderProgram, &uniform!{t: t, texSampler: &texture}, &Default::default()).unwrap();
		target.draw(voxels.GetVertices(), voxels.GetIndices(), &shaderProgram, &uniform!{t: t, texSampler: &texture, mat: mat.Content()}, &params).unwrap();

		target.finish().unwrap();

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
				_=> ()
			}
		}
	}
}
