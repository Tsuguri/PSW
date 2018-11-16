extern crate glium;


#[derive(Copy, Clone)]
pub enum KeyCode
{
	Key0 = 0,
	Key1 = 1,
	Key2 = 2,
	Key3 = 3,
	Key4 = 4,
	Key5 = 5,
	Key6 = 6,
	Key7 = 7,
	Key8 = 8,
	Key9 = 9,
	B = 10,
	C = 11,
	D = 12,
	E = 13,
	F = 14,
	G = 15,
	H = 16,
	I = 17,
	J = 18,
	K = 19,
	L = 20,
	M = 21,
	N = 22,
	O = 23,
	P = 24,
	Q = 25,
	R = 26,
	S = 27,
	T = 28,
	U = 29,
	V = 30,
	W = 31,
	X = 32,
	Y = 33,
	Z = 34,
	Escape = 35,
	F1 = 36,
	F2 = 37,
	F3 = 38,
	F4 = 39,
	F5 = 40,
	F6 = 41,
	F7 = 42,
	F8 = 43,
	F9 = 44,
	F10 = 45,
	F11 = 46,
	F12 = 47,
	F13 = 48,
	F14 = 49,
	F15 = 50,
	Left = 51,
	Up = 52,
	Right = 53,
	Down = 54,
	LControl = 55,
	LShift = 56,
	RControl = 57,
	RShift = 58,
}

impl KeyCode
{
	pub fn ToUsize(self) ->usize
	{
		match self
		{
			KeyCode::Key0 => 0,
			KeyCode::Key1 => 1,
			KeyCode::Key2 => 2,
			KeyCode::Key3 => 3,
			KeyCode::Key4 => 4,
			KeyCode::Key5 => 5,
			KeyCode::Key6 => 6,
			KeyCode::Key7 => 7,
			KeyCode::Key8 => 8,
			KeyCode::Key9 => 9,
			KeyCode::B => 10,
			KeyCode::C => 11,
			KeyCode::D => 12,
			KeyCode::E => 13,
			KeyCode::F => 14,
			KeyCode::G => 15,
			KeyCode::H => 16,
			KeyCode::I => 17,
			KeyCode::J => 18,
			KeyCode::K => 19,
			KeyCode::L => 20,
			KeyCode::M => 21,
			KeyCode::N => 22,
			KeyCode::O => 23,
			KeyCode::P => 24,
			KeyCode::Q => 25,
			KeyCode::R => 26,
			KeyCode::S => 27,
			KeyCode::T => 28,
			KeyCode::U => 29,
			KeyCode::V => 30,
			KeyCode::W => 31,
			KeyCode::X => 32,
			KeyCode::Y => 33,
			KeyCode::Z => 34,
			KeyCode::Escape => 35,
			KeyCode::F1 => 36,
			KeyCode::F2 => 37,
			KeyCode::F3 => 38,
			KeyCode::F4 => 39,
			KeyCode::F5 => 40,
			KeyCode::F6 => 41,
			KeyCode::F7 => 42,
			KeyCode::F8 => 43,
			KeyCode::F9 => 44,
			KeyCode::F10 => 45,
			KeyCode::F11 => 46,
			KeyCode::F12 => 47,
			KeyCode::F13 => 48,
			KeyCode::F14 => 49,
			KeyCode::F15 => 50,
			KeyCode::Left => 51,
			KeyCode::Up => 52,
			KeyCode::Right => 53,
			KeyCode::Down => 54,
			KeyCode::LControl => 55,
			KeyCode::LShift => 56,
			KeyCode::RControl => 57,
			KeyCode::RShift => 58
		}
	}
	pub fn FromGlutinEnum(value: glium::glutin::VirtualKeyCode) -> KeyCode
	{
		match value
		{
			glium::glutin::VirtualKeyCode::Key0 => KeyCode::Key0,
			glium::glutin::VirtualKeyCode::Key1 => KeyCode::Key1,
			glium::glutin::VirtualKeyCode::Key2 => KeyCode::Key2,
			glium::glutin::VirtualKeyCode::Key3 => KeyCode::Key3,
			glium::glutin::VirtualKeyCode::Key4 => KeyCode::Key4,
			glium::glutin::VirtualKeyCode::Key5 => KeyCode::Key5,
			glium::glutin::VirtualKeyCode::Key6 => KeyCode::Key6,
			glium::glutin::VirtualKeyCode::Key7 => KeyCode::Key7,
			glium::glutin::VirtualKeyCode::Key8 => KeyCode::Key8,
			glium::glutin::VirtualKeyCode::Key9 => KeyCode::Key9,
			glium::glutin::VirtualKeyCode::B => KeyCode::B,
			glium::glutin::VirtualKeyCode::C => KeyCode::C,
			glium::glutin::VirtualKeyCode::D => KeyCode::D,
			glium::glutin::VirtualKeyCode::E => KeyCode::E,
			glium::glutin::VirtualKeyCode::F => KeyCode::F,
			glium::glutin::VirtualKeyCode::G => KeyCode::G,
			glium::glutin::VirtualKeyCode::H => KeyCode::H,
			glium::glutin::VirtualKeyCode::I => KeyCode::I,
			glium::glutin::VirtualKeyCode::J => KeyCode::J,
			glium::glutin::VirtualKeyCode::K => KeyCode::K,
			glium::glutin::VirtualKeyCode::L => KeyCode::L,
			glium::glutin::VirtualKeyCode::M => KeyCode::M,
			glium::glutin::VirtualKeyCode::N => KeyCode::N,
			glium::glutin::VirtualKeyCode::O => KeyCode::O,
			glium::glutin::VirtualKeyCode::P => KeyCode::P,
			glium::glutin::VirtualKeyCode::Q => KeyCode::Q,
			glium::glutin::VirtualKeyCode::R => KeyCode::R,
			glium::glutin::VirtualKeyCode::S => KeyCode::S,
			glium::glutin::VirtualKeyCode::T => KeyCode::T,
			glium::glutin::VirtualKeyCode::U => KeyCode::U,
			glium::glutin::VirtualKeyCode::V => KeyCode::V,
			glium::glutin::VirtualKeyCode::W => KeyCode::W,
			glium::glutin::VirtualKeyCode::X => KeyCode::X,
			glium::glutin::VirtualKeyCode::Y => KeyCode::Y,
			glium::glutin::VirtualKeyCode::Z => KeyCode::Z,
			glium::glutin::VirtualKeyCode::Escape => KeyCode::Escape,
			glium::glutin::VirtualKeyCode::F1 => KeyCode::F1,
			glium::glutin::VirtualKeyCode::F2 => KeyCode::F2,
			glium::glutin::VirtualKeyCode::F3 => KeyCode::F3,
			glium::glutin::VirtualKeyCode::F4 => KeyCode::F4,
			glium::glutin::VirtualKeyCode::F5 => KeyCode::F5,
			glium::glutin::VirtualKeyCode::F6 => KeyCode::F6,
			glium::glutin::VirtualKeyCode::F7 => KeyCode::F7,
			glium::glutin::VirtualKeyCode::F8 => KeyCode::F8,
			glium::glutin::VirtualKeyCode::F9 => KeyCode::F9,
			glium::glutin::VirtualKeyCode::F10 => KeyCode::F10,
			glium::glutin::VirtualKeyCode::F11 => KeyCode::F11,
			glium::glutin::VirtualKeyCode::F12 => KeyCode::F12,
			glium::glutin::VirtualKeyCode::F13 => KeyCode::F13,
			glium::glutin::VirtualKeyCode::F14 => KeyCode::F14,
			glium::glutin::VirtualKeyCode::F15 => KeyCode::F15,
			glium::glutin::VirtualKeyCode::Left => KeyCode::Left,
			glium::glutin::VirtualKeyCode::Up => KeyCode::Up,
			glium::glutin::VirtualKeyCode::Right => KeyCode::Right,
			glium::glutin::VirtualKeyCode::Down => KeyCode::Down,
			glium::glutin::VirtualKeyCode::LControl => KeyCode::LControl,
			glium::glutin::VirtualKeyCode::LShift => KeyCode::LShift,
			glium::glutin::VirtualKeyCode::RControl => KeyCode::RControl,
			glium::glutin::VirtualKeyCode::RShift => KeyCode::RShift,
			_ => KeyCode::Key0
		}
	}
}

pub struct KeyboardState
{
	keys:  [bool; 256],
	oldKeys: [bool; 256]
}

impl KeyboardState
{
	pub fn SetButton(&mut self, key: KeyCode, value: bool)
	{
		self.keys[key.ToUsize()]=value;
	}

	pub fn IsButtonDown(&self, key: KeyCode) -> bool
	{
		self.keys[key.ToUsize()]
	}

	pub fn ButtonPressedInLastFrame(&self, key: KeyCode) -> bool
	{
		let k = key.ToUsize();
		self.keys[k] && !self.oldKeys[k]
	}

	pub fn NextFrame(&mut self)
	{
		self.oldKeys = self.keys;
		self.keys = [false; 256];
	}
}

impl Default for KeyboardState
{
	fn default() -> KeyboardState
	{
		KeyboardState
		{
			keys: [false; 256],
			oldKeys: [false; 256],
		}
	}
}
