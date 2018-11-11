extern crate glium;

#[derive(Copy, Clone)]
pub struct Vertex
{
	pub position: [f32; 3],
	pub normal: [f32; 3],
	pub tex: [f32; 2]
}
implement_vertex!(Vertex,position, normal, tex);

pub struct Model
{
	vertices: glium::VertexBuffer<Vertex>,
	indices: glium::index::IndexBuffer<u32>
}

impl Model
{
    pub fn GetVertices<'a>(&'a self) -> &'a glium::VertexBuffer<Vertex>
    {
        &self.vertices
    }

    pub fn GetIndices<'a>(&'a self)-> &'a glium::index::IndexBuffer<u32>
    {
        &self.indices
    }

    pub fn new(vertices: glium::VertexBuffer<Vertex>, indices: glium::index::IndexBuffer<u32>) -> Model
    {
        Model { vertices: vertices, indices: indices}
    }
}
