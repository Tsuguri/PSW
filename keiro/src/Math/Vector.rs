use std::ops::Mul;
use std::ops::Add;

#[derive(Copy, Clone)]
pub struct Vector4<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	content: [T; 4],
}

impl<T> Vector4<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	pub fn Sum(&self) -> T
	{
		self.content[0]+self.content[1]+self.content[2]+self.content[3]
	}
	
	pub fn new(x : T, y : T, z : T, w : T) -> Vector4<T>
	{
		Vector4 { content: [x, y, z, w]}
	}

	pub fn fromTab(tab: [T;4]) -> Vector4<T>
	{
		Vector4{content : tab}
	}
}

impl<T> Mul for Vector4<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	type Output = Vector4<T>;
	fn mul(self, rhs: Vector4<T>) -> Vector4<T>
	{
		Vector4::new(self.content[0]*rhs.content[0],self.content[1]*rhs.content[1],self.content[2]*rhs.content[2],self.content[3]*rhs.content[3])
	}
}