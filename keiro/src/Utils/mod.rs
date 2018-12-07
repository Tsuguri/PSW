pub fn LoadTexture(
    display: &glium::backend::Facade,
    fileName: &String,
) -> glium::texture::Texture2d {
    use std::path::Path;

    let image = image::open(&Path::new(fileName)).unwrap().to_rgba();

    let image_dimensions = image.dimensions();
    let image =
        glium::texture::RawImage2d::from_raw_rgba_reversed(image.into_raw(), image_dimensions);
    glium::texture::Texture2d::new(display, image).unwrap()
}

pub fn SaveTextureToFile(image: glium::texture::RawImage2d<u8>, name: &str) {
    //use std::path::Path;
    let image =
        image::ImageBuffer::from_raw(image.width, image.height, image.data.into_owned()).unwrap();
    let image = image::DynamicImage::ImageRgba8(image).flipv();
    image.save(name).unwrap();
}

pub fn LoadFileContent(filename: &String) -> String {
    use std::fs::File;
    use std::io::prelude::*;
    let mut file = File::open(filename).expect("Unable to open the file");
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Unable to read the file");
    contents
}
