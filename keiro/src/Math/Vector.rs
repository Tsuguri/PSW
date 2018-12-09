use std::ops::{Add, Index, IndexMut, Mul, Sub, Div};

use super::Matrix::Matrix4;

#[derive(Copy, Clone, Debug)]
pub struct Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    content: [T; 4],
}

impl<T> Default for Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T> + Default,
{
    fn default() -> Vector4<T> {
        Vector4 {
            content: [Default::default(); 4],
        }
    }
}

impl<T> Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    pub fn Sum(&self) -> T {
        self.content[0] + self.content[1] + self.content[2] + self.content[3]
    }

    pub fn new(x: T, y: T, z: T, w: T) -> Vector4<T> {
        Vector4 {
            content: [x, y, z, w],
        }
    }

    pub fn fromTab(tab: [T; 4]) -> Vector4<T> {
        Vector4 { content: tab }
    }
}

impl<T> Index<usize> for Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    type Output = T;

    fn index<'a>(&self, index: usize) -> &T {
        &self.content[index]
    }
}

impl<T> IndexMut<usize> for Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    fn index_mut<'a>(&mut self, index: usize) -> &mut T {
        &mut self.content[index]
    }
}

impl<T> Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T> + Default,
{
    pub fn matrix_multiply(vec1: Vector4<T>, vec2: Vector4<T>) -> Matrix4<T> {
        let mut mat = Matrix4::empty();

        for i in 0..4 {
            for j in 0..4 {
                mat[(i, j)] = vec1[i] * vec2[j];
            }
        }
        mat
    }
}

impl<T> Mul for Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    type Output = Vector4<T>;
    fn mul(self, rhs: Vector4<T>) -> Vector4<T> {
        Vector4::new(
            self.content[0] * rhs.content[0],
            self.content[1] * rhs.content[1],
            self.content[2] * rhs.content[2],
            self.content[3] * rhs.content[3],
        )
    }
}

impl<T> Add for Vector4<T>
where
    T: Copy + Add<Output = T> + Mul<Output = T>,
{
    type Output = Vector4<T>;
    fn add(self, rhs: Vector4<T>) -> Vector4<T> {
        Vector4::new(
            self.content[0] + rhs.content[0],
            self.content[1] + rhs.content[1],
            self.content[2] + rhs.content[2],
            self.content[3] + rhs.content[3],
        )
    }
}


pub trait VecElem : Copy + Add<Output=Self> + Mul<Output=Self> + Sub<Output=Self> + Div<Output=Self> + Default {
}

impl<T: Copy + Add<Output=T> + Mul<Output=T> + Sub<Output=T> + Div<Output=T> + Default> VecElem for T{}

#[derive(Copy, Clone, Debug)]
pub struct Vector3<T>
where
    T: VecElem
{
    content: [T; 3],
}

impl Vector3<f32> {
    pub fn normalized(&self) -> Self {
        let len = (self.x()*self.x() + self.y() * self.y() + self.z() * self.z()).sqrt();

        return Self::new(self.x()/len, self.y()/len, self.z()/len);
    }
}

impl<T> Vector3<T>
where
    T: VecElem
{
    pub fn Sum(&self) -> T {
        self.content[0] + self.content[1] + self.content[2]
    }

    pub fn empty() -> Vector3<T> {
        Vector3 {
            content: [Default::default(); 3],
        }
    }

    pub fn new(x: T, y: T, z: T) -> Vector3<T> {
        Vector3 { content: [x, y, z] }
    }

    pub fn fromTab(tab: [T; 3]) -> Vector3<T> {
        Vector3 { content: tab }
    }

    pub fn x(&self) -> T {
        self.content[0]
    }

    pub fn y(&self) -> T {
        self.content[1]
    }

    pub fn z(&self) -> T {
        self.content[2]
    }

    pub fn Content(&self) -> [T; 3] {
        self.content
    }

    pub fn cross(v1: &Self, v2: &Self) -> Self {
        Self::new(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x())
    }

}

impl<T> Mul<Vector3<T>> for Vector3<T>
where
    T: VecElem
{
    type Output = Vector3<T>;
    fn mul(self, rhs: Vector3<T>) -> Vector3<T> {
        Vector3::new(
            self.content[0] * rhs.content[0],
            self.content[1] * rhs.content[1],
            self.content[2] * rhs.content[2],
        )
    }
}

impl<T> Mul<T> for Vector3<T>
where
    T: VecElem
{
    type Output = Vector3<T>;
    fn mul(self, rhs: T) -> Vector3<T> {
        Vector3::new(
            self.content[0] * rhs,
            self.content[1] * rhs,
            self.content[2] * rhs,
        )
    }
}

impl<T> Add for Vector3<T>
where
    T: VecElem
{
    type Output = Vector3<T>;
    fn add(self, rhs: Vector3<T>) -> Vector3<T> {
        Vector3::new(
            self.content[0] + rhs.content[0],
            self.content[1] + rhs.content[1],
            self.content[2] + rhs.content[2],
        )
    }
}

impl<T> Sub for Vector3<T>
where
    T: VecElem
{
    type Output = Vector3<T>;
    fn sub(self, rhs: Vector3<T>) -> Vector3<T> {
       Vector3::new( 
            self.content[0] - rhs.content[0],
            self.content[1] - rhs.content[1],
            self.content[2] - rhs.content[2],
        )
    }
}
