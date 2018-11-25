use Math::Vector::Vector3;


fn move_tool(result : &mut Vec<Vector3<f32>>, j: u32, previous: i32, y: &mut i32, h: f32, g: &Fn(i32, i32)-> f32, floorOffset: f32, up: &mut bool, yToWorld: &Fn(i32)->f32, xToWorld: &Fn(i32)->f32, height: i32, width: i32, step: u32) {

        let mut x : i32= (j* step) as i32;
        if x>=width {
            x = width-1;
        }
        let xScene = xToWorld(x);
        println!("generating row {} at {} pixel with X: {}", j, x, xScene);


        let mut points : Vec<(i32, i32)>= vec![];


        points.push((x, *y));

        let mut prev = *y;
        loop {
            if *up{
                *y+=1;
            } else {
                *y-=1;
            }
            if *y>=height {
                *y = height-1;
                points.push((x, *y));
                break;
            }
            if *y<0 {
                *y = 0;
                points.push((x, *y));
                break;
            }

            let ch = g(x,*y);
            if(ch!=h){
                println!("not equal with: {}, {}", ch, h);
                points.push((x,prev));
                for step in 1..step {
                    println!("prev: {}, next: {}", previous, j);
                    let md : i32= if previous<j as i32 {1} else {-1};
                    let pt: i32 = x+(step as i32)*md as i32;
                    if pt>=width {
                        break;
                    }
                    // znalezienie najblizszej obok
                    let mut found = false;
                    let hh = g(pt, *y) == h;
                    for i in 1..1000 {
                        if *y+i<height {
                            let hp = g(pt, *y+i);
                            if *y + i<height && ((!hh && hp==h && !*up)||(hh && hp!=h && *up)) {
                                // jest ok w strone rufy
                                found = true;
                                *y= *y+i-(if *up {1} else {-1});
                                points.push((pt,*y));
                                break;
                            }
                        } else {
                            if *up {
                                found = true;
                                *y = height-1;
                                points.push((pt,*y));
                                break;
                            }
                        }
                        if *y-i>=0 {
                            let hd = g(pt, *y-i);
                            if *y - i>=0 && ((hh && hd!=h && !*up)|| (!hh && hd==h && *up)) {
                                //jest ok w strone rufy
                                found = true;
                                *y= *y-i + (if *up {-3} else {1});
                                points.push((pt,*y));
                                break;
                            }
                        } else {
                            if !*up {
                                found = true;
                                *y = 0;
                                points.push((pt, *y));
                                break;
                            }
                        }
                    }
                    if(!found){
                        println!("finished by panic at {}, {}!", pt, *y);
                        break;
                    }
                }
                // tutaj przechodzenie na sciezke obok
                break;
            }

            prev = *y;
        }
        *up = !*up;

        result.extend(points.iter().map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + h)));
}
pub fn generate_flat(l: f32, r:f32, b: f32, u: f32, floorOffset: f32, materialHeight: f32, toolRadius: f32, start: (f32,f32,f32), map: glium::texture::RawImage2d<u8>) ->Vec<Vector3<f32>> {
    let width = map.width as i32;
    let height = map.height as i32;
    let offWidth = (map.width/22) as i32;
    let offHeight = (map.height/22) as i32;
    let image = map.data.into_owned();
    let data : Vec<u8>= image.into_iter().step_by(4).collect();
    let data: Vec<f32>= data.iter().map(|x| (1.0-(*x as f32)/255.0)*10.0).collect();
    let sceneWidth = r-l;
    let sceneLength = u-b;

    let xToWorld = |x : i32| {
        ((x as f32)/(width as f32 - 1.0) * 1.04  - 0.02) * sceneWidth +l
        //((x as i32- offWidth as i32)as f32 / (width - 2*offWidth) as f32 * 0.9) //* sceneWidth + l
    };

    let yToWorld = |y : i32| {
        ((y as f32)/(height as f32 - 1.0) * 1.04 - 0.02) * sceneLength + b
        //((y as i32 - offHeight as i32) as f32 / (height - 2*offHeight) as f32 * 0.9) * sceneLength + b
    };

    let rows = ((r-l)/(1.8*toolRadius)).ceil() as u32;
    let step = (map.width as f32/ rows as f32).floor() as u32;

    println!("will make {} rows with step {}", rows, step);

    let mut result = vec![];
    result.push(Vector3::new(start.0, start.1, start.2));
    result.push(Vector3::new(-8.5, -8.5, materialHeight + 0.1));
    result.push(Vector3::new(-8.5, -8.5, floorOffset));

    /*println!("x0 = {}", xToWorld(0));
    println!("x0 = {}", xToWorld((width-1) as u32));
    println!("x0 = {}", xToWorld(offWidth as u32));
    println!("x0 = {}", xToWorld((width-1-offWidth) as u32));*/

    // tutaj generowanie plaskich sciezek
    

    let mut up = true;
    let mut y :i32=0;
        let g = |px : i32, py: i32|{
            data[(py*width + px) as usize]
        };
    let h = g(0, 0); // to jest docelowa wartosc wszedzie gdzie wchodzimy


    let mut prev =-1;
    for i in 0..(rows+1) {
        move_tool(&mut result, i, prev, &mut y, h, &g, floorOffset, &mut up, &yToWorld, &xToWorld, height, width, step);
        prev = i as i32;
    }

    for i in (0..rows).rev() {
        move_tool(&mut result, i, prev, &mut y, h, &g, floorOffset, &mut up, &yToWorld, &xToWorld, height, width, step);
        prev = i as i32;
    }

   

    let p = *result.last().unwrap();
    result.push(Vector3::new(p.x(), p.y(), materialHeight+1.0));
    result.push(Vector3::new(start.0, start.1, start.2));
    println!("generated {} points", result.len());

    result
}

pub fn generate_rough(l: f32, r:f32, b: f32, u: f32, floorOffset: f32, materialHeight: f32, toolRadius: f32, start: (f32,f32,f32), map: glium::texture::RawImage2d<u8>) -> Vec<Vector3<f32>> {

    let width = map.width;
    let height = map.height;
    let image = map.data.into_owned();
    let data : Vec<u8>= image.into_iter().step_by(4).collect();
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

        for j in (10..height).step_by(6) {
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
        points.push((x, height-1));
        upperPoints.push((x, height-1));

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
