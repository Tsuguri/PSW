extern crate glium;
use Math::Vector::Vector3;

#[derive(Copy, Clone)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
    pub tex: [f32; 2],
}
implement_vertex!(Vertex, position, normal, tex);

#[derive(Copy, Clone)]
pub struct PositionVertex {
    pub position: [f32; 3],
}
implement_vertex!(PositionVertex, position);

impl PositionVertex {
    pub fn new(vec: Vector3<f32>) -> PositionVertex {
        PositionVertex {
            position: [vec.x(), vec.y(), vec.z()],
        }
    }
}
pub struct Model<T>
where
    T: glium::vertex::Vertex,
{
    vertices: glium::VertexBuffer<T>,
    indices: glium::index::IndexBuffer<u32>,
}

impl<T> Model<T>
where
    T: glium::vertex::Vertex,
{
    pub fn GetVertices<'a>(&'a self) -> &'a glium::VertexBuffer<T> {
        &self.vertices
    }

    pub fn GetIndices<'a>(&'a self) -> &'a glium::index::IndexBuffer<u32> {
        &self.indices
    }

    pub fn new(
        vertices: glium::VertexBuffer<T>,
        indices: glium::index::IndexBuffer<u32>,
    ) -> Model<T> {
        Model {
            vertices: vertices,
            indices: indices,
        }
    }
}
