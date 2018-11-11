
use serde_xml_rs::deserialize;
use serde::{Deserializer, Deserialize};
use std::error::Error;


fn str_to_float<'de, D>(deserializer:D) -> Result<f64, D::Error> where D: Deserializer<'de> {
    let s = String::deserialize(deserializer)?;
    s.parse::<f64>().map_err(serde::de::Error::custom)
}
fn str_to_u32<'de, D>(deserializer:D) -> Result<u32, D::Error> where D: Deserializer<'de> {
    let s = String::deserialize(deserializer)?;
    s.parse::<u32>().map_err(serde::de::Error::custom)
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Vec3 {
    #[serde(rename="X", deserialize_with = "str_to_float")]
    x: f64,
    #[serde(rename="Y", deserialize_with = "str_to_float")]
    y: f64,
    #[serde(rename="Z", deserialize_with = "str_to_float")]
    z: f64,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Point {
    #[serde(rename="Name")]
    name: String,
    #[serde(rename="Position")]
    pos: Vec3,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Uvs {
    #[serde(deserialize_with="str_to_u32")]
    PointU0V0: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU1V0: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU2V0: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU3V0: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU0V1: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU1V1: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU2V1: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU3V1: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU0V2: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU1V2: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU2V2: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU3V2: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU0V3: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU1V3: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU2V3: u32,
    #[serde(deserialize_with="str_to_u32")]
    PointU3V3: u32,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Patches {
    #[serde(rename="BezierSurfaceC2Patch")]
    patches: Vec<Patch>
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Patch {
    #[serde(rename="Name")]
    name: String,
    #[serde(rename="PatchU", deserialize_with="str_to_u32")]
    u: u32,
    #[serde(rename="PatchV", deserialize_with="str_to_u32")]
    v: u32,
    #[serde(rename="Points")]
    uvs: Uvs
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Surface {
    #[serde(rename="Name")]
    name: String,
    #[serde(rename="PatchesU", deserialize_with="str_to_u32")]
    patchesU: u32,
    #[serde(rename="PatchesV", deserialize_with="str_to_u32")]
    patchesV: u32,
    #[serde(rename="Patches")]
    patches: Patches
}


#[derive(Serialize, Deserialize, Debug)]
pub struct Pts {
    #[serde(rename="Point")]
    pts: Vec<Point>
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Surfaces {
    #[serde(rename="BezierSurfaceC2")]
    surfaces: Vec<Surface>

}

#[derive(Serialize, Deserialize, Debug)]
pub struct Scene {
    #[serde(rename="Points")]
    points: Pts,
    #[serde(rename="BezierSurfacesC2")]
    surfaces: Surfaces
}

pub fn deser(path: &str) -> Scene {
	use std::fs::File;
	use std::io::prelude::*;
    let mut file = File::open(path).expect("Unable to open the file");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Unable to read the file");
    match deserialize::<&[u8], Scene>(contents.as_bytes()) {
        Ok(val) => {
            println!("{:?}", &val.points);
            println!("{:?}", &val.surfaces);
            val   
        },
        Err(err) => {
            println!("{:?}",err);
            /*match err {
                ParseIntError(err2) => {
                    println!("{}"
                }
            }*/
            panic!();
        }
    }
}
