use Math::Matrix::Matrix4;
use Math::Vector::Vector3;

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
	fov : f32,
	changed : bool
}

impl Camera
{
	pub fn new(x: f32, y : f32, target : Vector3<f32>, rad : f32, fov : f32, zNear: f32, zFar: f32, ratio: f32) -> Camera
	{
		Camera{target : target, xAngle : x, yAngle : y, radius : rad, zNear: zNear, zFar : zFar, aspectRatio : ratio, fov : fov, changed : true}
	}

	pub fn ViewMatrix(target: Vector3<f32>, xang : f32, yang: f32, rad : f32) -> Matrix4<f32>
	{
		Matrix4::Translation(0.0, 0.0, rad) * Matrix4::RotationX(xang/180.0 * 3.14) * Matrix4::RotationY(yang/180.0 * 3.14) * Matrix4::Translation(target.x(), target.y(), target.z())
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
		self.changed = true;
	}

	pub fn ModifyYAngle(&mut self, dY : f32)
	{
		let ang = self.yAngle + dY;
		self.SetYAngle(ang);
	}

	pub fn SetXAngle(&mut self, angle : f32)
	{
		self.xAngle = angle;
		self.changed = true;
		if self.xAngle > 90.0
		{
			self.xAngle = 90.0;
		}
		if self.xAngle < -90.0
		{
			self.xAngle = -90.0;
		}
	}

	pub fn ModifyXAngle(&mut self, dX : f32)
	{
		let ang = self.xAngle + dX;
		self.SetXAngle(ang);
	}

	pub fn SetRadius(&mut self, radius : f32)
	{
		self.radius = radius;
		self.changed = true;
				if self.radius < 0.1
		{
			self.radius = 0.1;
		}
	}

	pub fn ModifyRadius(&mut self, dR : f32)
	{
		let rad = self.radius + dR;
		self.SetRadius(rad);
	}

	pub fn GetPosition(&self) -> Vector3<f32>
	{
		let yang = (self.yAngle - 90.0)/180.0 * 3.14;
		let xang = (-self.xAngle - 90.0)/180.0 * 3.14;
		let x = yang.cos() * (xang).sin();
		let z = yang.sin() * (xang).sin();
		let y = (xang).cos();

		self.target + Vector3::new(-x,y,-z) * self.radius
	}

    pub fn PrintData(&self) {
        println!(" radius: {}, yang: {}, xang: {}",self.radius, self.yAngle, self.xAngle);
    }
	
}
