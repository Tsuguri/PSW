use std::ops::Mul;
use std::ops::Add;
use std::convert::From;

use Math::Vector::*;

#[derive(Copy, Clone)]
pub struct Matrix4<T> where T: Mul<Output = T> + Add<Output = T> + Copy
{
	content: [[T; 4]; 4],
}



impl<T> Matrix4<T> where T: Mul<Output = T> + Add<Output = T> + Copy
{
	pub fn new(v00 : T, v01 : T, v02 : T, v03 : T,
				v10 : T, v11 : T, v12 : T, v13 : T,
				v20 : T, v21 : T, v22 : T, v23 : T,
				v30 : T, v31 : T, v32 : T, v33 : T) -> Matrix4<T>
	{
		Matrix4 {
			content : [[v00,v01,v02,v03],[v10,v11,v12,v13],[v20,v21,v22,v23],[v30,v31,v32,v33]],
		}
	}


	pub fn Content(&self) -> [[T; 4]; 4]
	{
		self.content
	}

#[allow(dead_code)]
	fn GetContent<'a>(&'a self) -> &'a [[T; 4]; 4]
	{
		&self.content
	}

	pub fn GetRow(&self, index : usize) -> Vector4<T>
	{
		if index > 3
		{
			panic!("Matrix4 index out of range!");
		}
		
		Vector4::fromTab(self.content[index])
	}

	pub fn GetColumn(&self, index : usize) -> Vector4<T>
	{
		if index > 3
		{
			panic!("Matrix4 index out of range!");
		}
		Vector4::new(self.content[0][index], self.content[1][index], self.content[2][index], self.content[3][index])
	}

	pub fn Transposed(&self) ->Matrix4<T>
	{
		Matrix4::new(
			self.content[0][0],self.content[1][0],self.content[2][0],self.content[3][0],
			self.content[0][1],self.content[1][1],self.content[2][1],self.content[3][1],
			self.content[0][2],self.content[1][2],self.content[2][2],self.content[3][2],
			self.content[0][3],self.content[1][3],self.content[2][3],self.content[3][3],
		)
	}
}

impl<T> Mul for Matrix4<T> where T : Mul<Output = T> + Add<Output = T> + Copy
{
	type Output = Self;
	fn mul(self, rhs: Self ) -> Self
	{
		let r0 = self.GetRow(0);
		let r1 = self.GetRow(1);
		let r2 = self.GetRow(2);
		let r3 = self.GetRow(3);
		let c0 = rhs.GetColumn(0);
		let c1 = rhs.GetColumn(1);
		let c2 = rhs.GetColumn(2);
		let c3 = rhs.GetColumn(3);
		Matrix4 {content: [
			[(r0*c0).Sum(), (r0*c1).Sum(), (r0*c2).Sum(), (r0*c3).Sum()],
			[(r1*c0).Sum(), (r1*c1).Sum(), (r1*c2).Sum(), (r1*c3).Sum()],
			[(r2*c0).Sum(), (r2*c1).Sum(), (r2*c2).Sum(), (r2*c3).Sum()],
			[(r3*c0).Sum(), (r3*c1).Sum(), (r3*c2).Sum(), (r3*c3).Sum()]
		]
		}
	}
}

impl<T> Matrix4<T> where T: From<f32> + Mul<Output = T> + Add<Output = T> + Copy
{
	pub fn RotationX(angle : f32) -> Matrix4<T>
	{
		let cos = angle.cos();
		let sin = angle.sin();
		Matrix4::new(
			T::from(1.0), T::from(0.0), T::from(0.0), T::from(0.0),
			T::from(0.0), T::from(cos), T::from(-sin), T::from(0.0),
			T::from(0.0), T::from(sin), T::from(cos), T::from(0.0),
			T::from(0.0), T::from(0.0), T::from(0.0), T::from(1.0)
		)
	}

	pub fn RotationY(angle : f32) -> Matrix4<T>
	{
		let cos = angle.cos();
		let sin = angle.sin();
		Matrix4::new(
			T::from(cos), T::from(0.0), T::from(sin), T::from(0.0),
			T::from(0.0), T::from(1.0), T::from(0.0), T::from(0.0),
			T::from(-sin), T::from(0.0), T::from(cos), T::from(0.0),
			T::from(0.0), T::from(0.0), T::from(0.0), T::from(1.0)
		)
	}

	pub fn Translation(x : f32, y : f32, z : f32) -> Matrix4<T>
	{
		Matrix4::new(
			T::from(1.0), T::from(0.0), T::from(0.0), T::from(x),
			T::from(0.0), T::from(1.0), T::from(0.0), T::from(y),
			T::from(0.0), T::from(0.0), T::from(1.0), T::from(z),
			T::from(0.0), T::from(0.0), T::from(0.0), T::from(1.0)
		)
	}

	pub fn Frustum(fovy : f32, aspect : f32, near : f32, far : f32) -> Matrix4<T>
	{
		let cot = 1.0 / (fovy/2.0).tan();
		Matrix4::new(
			T::from(cot * aspect), T::from(0.0), T::from(0.0), T::from(0.0),
            T::from(0.0), T::from(cot), T::from(0.0), T::from(0.0),
            T::from(0.0), T::from(0.0), T::from((far+near)/(far-near)), T::from(-(2.0*far*near)/(far-near)),
            T::from(0.0), T::from(0.0), T::from(1.0), T::from(0.0) 
		)
	}
}
