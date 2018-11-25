use Math::Vector::Vector3;

pub fn generate_flat(l: f32, r:f32, b: f32, u: f32, floorOffset: f32, materialHeight: f32, toolRadius: f32, start: (f32,f32,f32), map: glium::texture::RawImage2d<u8>) ->Vec<Vector3<f32>> {
    vec![]
}

pub fn generate_rough(l: f32, r:f32, b: f32, u: f32, floorOffset: f32, materialHeight: f32, toolRadius: f32, start: (f32,f32,f32), map: glium::texture::RawImage2d<u8>) -> Vec<Vector3<f32>> {

    let width = map.width;
    let height = map.height;
    let image = map.data.into_owned();
    let data : Vec<u8>= image.iter().enumerate().step_by(4).map(|x|*x.1).collect();
    let data: Vec<f32>= data.iter().map(|x| (1.0-(*x as f32)/255.0)*10.0).collect();
    let sceneWidth = r-l;
    let sceneLength = u-b;
    println!("first: {}", data[0]);

    let xToWorld = |x : u32| {
        (x as f32 / width as f32) * sceneWidth + l
    };

    let yToWorld = |y : u32| {
        (y as f32 / height as f32) * sceneLength + b
    };

    let rows = ((r-l)/(1.1*toolRadius)).ceil() as u32;
    let step = (map.width as f32/ rows as f32).floor() as u32;
    println!("generating {} rows", rows);

    let mut result = vec![];
    let mut upperResult = vec![];
    upperResult.push(Vector3::new(start.0, start.1, start.2));
    upperResult.push(Vector3::new(-8.0, -8.0, 6.0));
    result.push(Vector3::new(-8.0, -8.0, 6.0));

    // generate from starto to (l,b) here
    // dolna warstwa
    for i in 0..(rows+1) {
        let mut x = (i* step);
        if x>=width {
            x = width-1;
        }
        let x = x;
        let xScene = (i * step) as f32/(width as f32) * sceneWidth + l;
        println!("generating row {} at {} pixel with X: {}", i, x, xScene);


        let mut points = vec![];
        let mut upperPoints = vec![];
        let mut prev = 0;

        let g = |y : u32|{
            data[(y*width + x) as usize]
        };
        let upper_g = |y: u32| {
            f32::max(2.3, data[(y*width + x) as usize])
        };
        let mut prevPt = g(0);
        let mut upperPrev = upper_g(0);

        // starting point of pass
        points.push((x, 0));
        upperPoints.push((x, 0));

        for j in (10..height).step_by(10) {
            let h = g(j);
            let h2 = upper_g(j);
        
            if(h!=prevPt){
                points.push((x,j));
            }

            if(h2!=upperPrev){
                upperPoints.push((x,j));
            }

            prev = j;
            prevPt = h;
            upperPrev = h2;
        }
        points.push((x, 2999));
        upperPoints.push((x, 2999));

        if i%2==0 {
            result.extend(points.iter().map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + g(pt.1)- toolRadius)));
            upperResult.extend(upperPoints.iter().map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + upper_g(pt.1)- toolRadius)));
        } else {
            result.extend(points.iter().rev().map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + g(pt.1)- toolRadius)));
            upperResult.extend(upperPoints.iter().rev().map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + upper_g(pt.1)- toolRadius)));
        };
    }

    result.push(Vector3::new(8.0, 8.0, 6.0));
    result.push(Vector3::new(-8.0, -8.0, 6.0));
    result.push(Vector3::new(start.0, start.1, start.2));
    upperResult.push(Vector3::new(8.0, 8.0, 6.0));
    println!("upper: {}, lower: {}", upperResult.len(), result.len());

    upperResult.extend(result.iter());
    println!("Result size: {}", upperResult.len());

    upperResult
}
