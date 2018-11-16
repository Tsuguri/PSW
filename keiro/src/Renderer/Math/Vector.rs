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
		Vector4::new(self.content[0] * rhs.content[0], self.content[1] * rhs.content[1], self.content[2] * rhs.content[2], self.content[3] * rhs.content[3])
	}
}

impl<T> Add for Vector4<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	type Output = Vector4<T>;
	fn add(self, rhs: Vector4<T>) -> Vector4<T>
	{
		Vector4::new(self.content[0] + rhs.content[0], self.content[1] + rhs.content[1], self.content[2] + rhs.content[2], self.content[3] + rhs.content[3])
	}
}



#[derive(Copy, Clone)]
pub struct Vector3<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	content: [T; 3],
}

impl<T> Vector3<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	pub fn Sum(&self) -> T
	{
		self.content[0]+self.content[1]+self.content[2]
	}
	
	pub fn new(x : T, y : T, z : T) -> Vector3<T>
	{
		Vector3 { content: [x, y, z]}
	}

	pub fn fromTab(tab: [T;3]) -> Vector3<T>
	{
		Vector3{content : tab}
	}

	pub fn x(&self) -> T
	{
		self.content[0]
	}

	pub fn y(&self) -> T
	{
		self.content[1]
	}

	pub fn z(&self) -> T
	{
		self.content[2]
	}

	pub fn Content(&self) -> [T; 3]
	{
		self.content
	}
}

impl<T> Mul<Vector3<T>> for Vector3<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	type Output = Vector3<T>;
	fn mul(self, rhs: Vector3<T>) -> Vector3<T>
	{
		Vector3::new(self.content[0] * rhs.content[0], self.content[1] * rhs.content[1], self.content[2] * rhs.content[2])
	}
}

impl<T> Mul<T> for Vector3<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	type Output = Vector3<T>;
	fn mul(self, rhs: T) -> Vector3<T>
	{
		Vector3::new(self.content[0] * rhs, self.content[1]*rhs, self.content[2]*rhs)
	}
}

impl<T> Add for Vector3<T> where T: Copy + Add<Output = T> + Mul<Output = T>
{
	type Output = Vector3<T>;
	fn add(self, rhs: Vector3<T>) -> Vector3<T>
	{
		Vector3::new(self.content[0] + rhs.content[0], self.content[1] + rhs.content[1], self.content[2] + rhs.content[2])
	}
}