use serde::{Deserialize, Deserializer};
use serde_xml_rs::deserialize;
use std::error::Error;

use Math::Vector::Vector3;
use Math::Vector::Vector4;

fn str_to_float<'de, D>(deserializer: D) -> Result<f32, D::Error>
where
    D: Deserializer<'de>,
{
    let s = String::deserialize(deserializer)?;
    s.parse::<f32>().map_err(serde::de::Error::custom)
}
fn str_to_u32<'de, D>(deserializer: D) -> Result<u32, D::Error>
where
    D: Deserializer<'de>,
{
    let s = String::deserialize(deserializer)?;
    s.parse::<u32>().map_err(serde::de::Error::custom)
}

#[derive(Serialize, Deserialize, Debug)]
struct Vec3 {
    #[serde(rename = "X", deserialize_with = "str_to_float")]
    x: f32,
    #[serde(rename = "Y", deserialize_with = "str_to_float")]
    y: f32,
    #[serde(rename = "Z", deserialize_with = "str_to_float")]
    z: f32,
}

impl Vec3 {
    pub fn to_vector(&self) -> Vector3<f32> {
        Vector3::new(self.x, self.y, self.z)
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct Point {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "Position")]
    pos: Vec3,
}

#[derive(Serialize, Deserialize, Debug)]
struct Uvs {
    #[serde(deserialize_with = "str_to_u32")]
    PointU0V0: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU1V0: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU2V0: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU3V0: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU0V1: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU1V1: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU2V1: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU3V1: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU0V2: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU1V2: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU2V2: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU3V2: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU0V3: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU1V3: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU2V3: u32,
    #[serde(deserialize_with = "str_to_u32")]
    PointU3V3: u32,
}

#[derive(Serialize, Deserialize, Debug)]
struct Patches {
    #[serde(rename = "BezierSurfaceC2Patch")]
    patches: Vec<InternalPatch>,
}

#[derive(Serialize, Deserialize, Debug)]
struct InternalPatch {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "PatchU", deserialize_with = "str_to_u32")]
    u: u32,
    #[serde(rename = "PatchV", deserialize_with = "str_to_u32")]
    v: u32,
    #[serde(rename = "Points")]
    uvs: Uvs,
}

#[derive(Serialize, Deserialize, Debug)]
struct InternalSurface {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "PatchesU", deserialize_with = "str_to_u32")]
    patchesU: u32,
    #[serde(rename = "PatchesV", deserialize_with = "str_to_u32")]
    patchesV: u32,
    #[serde(rename = "Patches")]
    patches: Patches,
}

fn TransformPatch(patch: &Uvs, pts: &Vec<Point>) -> Patch {
    let p = [
        [
            pts[patch.PointU0V0 as usize].pos.to_vector(),
            pts[patch.PointU0V1 as usize].pos.to_vector(),
            pts[patch.PointU0V2 as usize].pos.to_vector(),
            pts[patch.PointU0V3 as usize].pos.to_vector(),
        ],
        [
            pts[patch.PointU1V0 as usize].pos.to_vector(),
            pts[patch.PointU1V1 as usize].pos.to_vector(),
            pts[patch.PointU1V2 as usize].pos.to_vector(),
            pts[patch.PointU1V3 as usize].pos.to_vector(),
        ],
        [
            pts[patch.PointU2V0 as usize].pos.to_vector(),
            pts[patch.PointU2V1 as usize].pos.to_vector(),
            pts[patch.PointU2V2 as usize].pos.to_vector(),
            pts[patch.PointU2V3 as usize].pos.to_vector(),
        ],
        [
            pts[patch.PointU3V0 as usize].pos.to_vector(),
            pts[patch.PointU3V1 as usize].pos.to_vector(),
            pts[patch.PointU3V2 as usize].pos.to_vector(),
            pts[patch.PointU3V3 as usize].pos.to_vector(),
        ],
    ];
    Patch { points: p }
}

fn TransformSurface(surface: &InternalSurface, pts: &Pts) -> Surface {
    println!("transforming: {}", surface.name);
    let u = surface.patchesU;
    let v = surface.patchesV;
    let mut patches = vec![Patch::new(); (u * v) as usize];

    for patch in &surface.patches.patches {
        patches[(patch.u * v + patch.v) as usize] = TransformPatch(&patch.uvs, &pts.pts);
    }
    Surface { u, v, patches }
}

#[derive(Serialize, Deserialize, Debug)]
struct Pts {
    #[serde(rename = "Point")]
    pts: Vec<Point>,
}

#[derive(Serialize, Deserialize, Debug)]
struct Surfaces {
    #[serde(rename = "BezierSurfaceC2")]
    surfaces: Vec<InternalSurface>,
}

#[derive(Serialize, Deserialize, Debug)]
#[serde(rename = "Scene")]
struct InternalScene {
    #[serde(rename = "Points")]
    points: Pts,
    #[serde(rename = "BezierSurfacesC2")]
    surfaces: Surfaces,
}

fn TransformScene(scene: &InternalScene) -> Scene {
    let mut surfaces = vec![];
    println!("start");
    for p in &scene.surfaces.surfaces {
        surfaces.push(TransformSurface(p, &scene.points));
    }
    Scene { surfaces }
}

pub fn deser(path: &str) -> Scene {
    use std::fs::File;
    use std::io::prelude::*;
    let mut file = File::open(path).expect("Unable to open the file");
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Unable to read the file");
    match deserialize::<&[u8], InternalScene>(contents.as_bytes()) {
        Ok(val) => {
            //println!("{:?}", &ret.surfaces);
            //println!("{:?}", &val.surfaces);
            TransformScene(&val)
        }
        Err(err) => {
            println!("{:?}", err);
            /*match err {
                ParseIntError(err2) => {
                    println!("{}"
                }
            }*/
            panic!();
        }
    }
}

#[derive(Copy, Clone, Debug)]
pub struct Patch {
    pub points: [[Vector3<f32>; 4]; 4],
}

impl Patch {
    pub fn new() -> Patch {
        Patch {
            points: [[Vector3::<f32>::empty(); 4]; 4],
        }
    }
    /*
var n = new Vector4 { [0] = 1.0 };
			double tm = 1.0 - t;
			for (int j = 1; j <= degree; j++)
			{
				double saved = 0.0;
				for (int k = 1; k <= j; k++)
				{
					double term = n[k - 1] / ((tm + k - 1.0) + (t + j - k));
					n[k - 1] = saved + (tm + k - 1.0) * term;
					saved = (t + j - k) * term;
				}
				n[j] = saved;
			}*/
    fn eval_bspline(t: f32, degree: usize) -> Vector4<f32> {
        let mut vec: Vector4<f32> = Default::default();
        vec[0] = 1.0;
        let tm = 1.0 - t;
        for j in 1..degree + 1 {
            let mut saved = 0.0;
            for k in 1..j + 1 {
                //println!("{}, {}", j, k);
                let term = vec[k - 1] / ((tm + k as f32 - 1.0) + (t + j as f32 - k as f32));
                //println!("{}", term);
                vec[k - 1] = saved + (tm + k as f32 - 1.0) * term;
                saved = (t + (j as f32) - (k as f32)) * term;
            }
            //println!("{}", saved);
            vec[j] = saved;
        }
        return vec;
    }

    pub fn evaluate(&self, u: f32, v: f32) -> Vector3<f32> {
        //println!("evaluating at: {}, {}", u, v);
        let fu = Self::eval_bspline(u, 3);
        let fv = Self::eval_bspline(v, 3);
        //println!("with: {:?}, {:?}", fu, fv);

        let fs = Vector4::matrix_multiply(fu, fv);
        //println!("{:?}", fs);

        let mut result = Vector3::empty();
        for i in 0..4 {
            for j in 0..4 {
                let increase = self.points[i][j] * fs[(i, j)];
                result = result + increase;
                //println!("inc: {:?}, res: {:?}", increase, result);
            }
        }
        result
    }
}

#[derive(Clone, Debug)]
pub struct Surface {
    pub u: u32,
    pub v: u32,
    pub patches: Vec<Patch>,
}

impl Surface {
    pub fn evaluate(&self, u: f32, v: f32) -> Vector3<f32> {
        let pu = u.trunc() as usize;
        let pv = v.trunc() as usize;
        let du = u.fract();
        let dv = v.fract();
        self.patches[pu * self.v as usize + pv].evaluate(du, dv)
    }
}

pub struct Scene {
    pub surfaces: Vec<Surface>,
}
