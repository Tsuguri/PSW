extern crate cgmath;


use Math::Matrix::Matrix4;
use self::cgmath::Vector3;

#[derive(Copy, Clone)]
pub struct Camera
{
	target : Vector3<f32>,
	xAngle : f32,
	yAngle : f32,
	radius : f32,

	zNear : f32,
	zFar : f32,
	aspectRatio : f32,
	fov : f32
}

impl Camera
{
	pub fn new(x: f32, y : f32, target : Vector3<f32>, rad : f32, fov : f32, zNear: f32, zFar: f32, ratio: f32) -> Camera
	{
		Camera{target : target, xAngle : x, yAngle : y, radius : rad, zNear: zNear, zFar : zFar, aspectRatio : ratio, fov : fov}
	}

	fn ViewMatrix(_target: Vector3<f32>, xang : f32, yang: f32, rad : f32) -> Matrix4<f32>
	{
		 Matrix4::Translation(0.0, 0.0, rad) * Matrix4::RotationX(xang/180.0 * 3.14) * Matrix4::RotationY(yang/180.0 * 3.14)
	}

	fn ProjectionMatrix(fov : f32, zNear : f32, zFar : f32, aspect: f32) -> Matrix4<f32>
	{
		Matrix4::Frustum(fov, aspect, zNear, zFar)
	}

	pub fn GetViewMatrix(&self) -> Matrix4<f32>
	{
		Camera::ViewMatrix(self.target, self.xAngle, self.yAngle, self.radius)
	}

	pub fn GetProjectionMatrix(&self) -> Matrix4<f32>
	{
		Camera::ProjectionMatrix(self.fov, self.zNear, self.zFar, self.aspectRatio)
	}

	pub fn SetYAngle(&mut self, angle : f32)
	{
		self.yAngle = angle;
	}

#[allow(dead_code)]
	pub fn SetXAngle(&mut self, angle : f32)
	{
		self.xAngle = angle;
	}

#[allow(dead_code)]
	pub fn SetRadius(&mut self, radius : f32)
	{
		self.radius = radius;
	}
	
}
