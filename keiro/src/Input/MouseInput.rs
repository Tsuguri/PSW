extern crate glium;

pub struct MouseState {
    pub mouseMoved: bool,
    pub currentPosition: [f32; 2],
    pub previousPosition: [f32; 2],
    pub buttonPressed: [bool; 8],
}

impl MouseState {
    pub fn GetMouseMove(&self) -> [f32; 2] {
        if self.mouseMoved {
            [
                self.currentPosition[0] - self.previousPosition[0],
                self.currentPosition[1] - self.previousPosition[1],
            ]
        } else {
            [0.0, 0.0]
        }
    }

    pub fn LeftButtonDown(&self) -> bool {
        self.buttonPressed[0]
    }

    pub fn RightButtonDown(&self) -> bool {
        self.buttonPressed[1]
    }

    pub fn SetButtonState(&mut self, button: usize, value: bool) {
        self.buttonPressed[button] = value;
    }

    pub fn SetNewPosition(&mut self, newPosition: [f32; 2]) {
        self.mouseMoved = true;
        self.previousPosition = self.currentPosition;
        self.currentPosition = newPosition;
    }
}

impl Default for MouseState {
    fn default() -> MouseState {
        MouseState {
            currentPosition: [0.0; 2],
            previousPosition: [0.0; 2],
            buttonPressed: [false; 8],
            mouseMoved: false,
        }
    }
}
