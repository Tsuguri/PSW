use Data;
use Math::Vector::Vector3;
use Math::Vector::Vector2;

fn move_tool(
    result: &mut Vec<Vector3<f32>>,
    j: i32,
    previous: i32,
    y: &mut i32,
    h: f32,
    g: &Fn(i32, i32) -> f32,
    floorOffset: f32,
    up: &mut bool,
    yToWorld: &Fn(i32) -> f32,
    xToWorld: &Fn(i32) -> f32,
    height: i32,
    width: i32,
    step: u32,
    swap: bool,
) {
    let mut x: i32 = j;
    if x >= width {
        x = width - 1;
    }
    let xScene = xToWorld(x);
    println!("generating row {} at {} pixel with X: {}", j, y, xScene);

    let mut points: Vec<(i32, i32)> = vec![];

    points.push((x, *y));

    let mut prev = *y;
    loop {
        if *up {
            *y += 1;
        } else {
            *y -= 1;
        }
        if *y >= height {
            *y = height - 1;
            points.push((x, *y));
            break;
        }
        if *y < 0 {
            *y = 0;
            points.push((x, *y));
            //println!("y below 0");
            break;
        }

        let ch = g(x, *y);
        if (ch != h) {
            //println!("not equal with: {}, {}", ch, h);
            points.push((x, prev));
            for step in 1..step {
                //println!("prev: {}, next: {}", previous, j);
                let md: i32 = if previous < j as i32 { 1 } else { -1 };
                let pt: i32 = x + (step as i32) * md as i32;
                if pt >= width {
                    break;
                }
                // znalezienie najblizszej obok
                let mut found = false;
                let hh = g(pt, *y) == h;
                for i in 1..1000 {
                    if *y + i < height {
                        let hp = g(pt, *y + i);
                        if *y + i < height && ((!hh && hp == h && !*up) || (hh && hp != h && *up)) {
                            // jest ok w strone rufy
                            found = true;
                            *y = *y + i - (if *up { 1 } else { 0 });
                            points.push((pt, *y));
                            //println!("y option 1");
                            break;
                        }
                    } else {
                        if *up {
                            found = true;
                            *y = height - 1;
                            points.push((pt, *y));
                            //println!("y option 2");
                            break;
                        }
                    }
                    if *y - i >= 0 {
                        let hd = g(pt, *y - i);
                        if *y - i >= 0 && ((hh && hd != h && !*up) || (!hh && hd == h && *up)) {
                            //jest ok w strone rufy
                            found = true;
                            *y = *y - i + (if *up { 0 } else { 1 });
                            points.push((pt, *y));
                            //println!("y option 3");
                            break;
                        }
                    } else {
                        if !*up {
                            found = true;
                            *y = 0;
                            points.push((pt, *y));
                            //println!("y option 4");
                            break;
                        }
                    }
                }
                if (!found) {
                    points.push((pt, *y));
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
    if swap {
        for pt in &mut points {
            std::mem::swap(&mut pt.0, &mut pt.1);
        }
    }

    result.extend(
        points
            .iter()
            .map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + h)),
    );
}

fn generate_bool_map(width: i32, height: i32, result: &mut [bool], u: u32, v: u32, path: &[Vector2<f32>], inv: bool) {


}

fn generate_height_map(result: &mut [bool], u: i32, v: i32, surface: &Data::Surface, toolRadius: f32) {
    for up in 0..u {
        for vp in 0..v {

            let uc =(surface.u as i32*up) as f32 / (u as f32); 
            let vc = (surface.v as i32*vp) as f32 / (v as f32);
            let q = surface.eval_dist(uc, vc, toolRadius);
            if q.y()<toolRadius {
                result[(up*v+vp) as usize] = false;
            }
        }
    }
}

fn generate_gatling_details(gatling: &Data::Surface, gatling_wing: &[Vector2<f32>], toolRadius: f32) -> Vec<Vector3<f32>> {

    let uRes :i32 = 130;
    let vRes  :i32= 180;

    let mut map = vec![true; (uRes*vRes) as usize];


    generate_height_map(&mut map, uRes, vRes, gatling, toolRadius);
    generate_bool_map(uRes, vRes, &mut map, gatling.u, gatling.v, gatling_wing, false);
    let value = |up: i32,vp:i32| {
        map[(up*vRes+vp) as usize]
    };
    let mut result = vec![];

    for u in 0..uRes {
        let rev = u%2==0;
        for v in 1..vRes {
            let val = if rev { vRes-v} else {v};
            if !value(u,val) {
                continue;
            }
            let uc =(gatling.u as i32*u) as f32 / (uRes as f32); 
            let vc = (gatling.v as i32*val) as f32 / (vRes as f32);
            let q = gatling.eval_dist(
                uc,vc
                , toolRadius);
            result.push(q);
        }
    }
    result

    //gatling_wing.iter().map(|x| gatling.eval_dist(x[0], x[1], toolRadius)).collect()
}

pub fn generate_details(
    l: f32,
    r: f32,
    b: f32,
    u: f32,
    floorOffset: f32,
    materialHeight: f32,
    toolRadius: f32,
    start: (f32, f32, f32),
    map: glium::texture::RawImage2d<u8>,
    d: &Data::Scene,
) -> Vec<Vector3<f32>> {
    // 0 -> gatling
    // 1 -> body
    // 2 -> cockpit
    // 3 -> wings
    let gatling = &d.surfaces[0];
    let body = &d.surfaces[1];
    let cockpit = &d.surfaces[2];
    let wings = &d.surfaces[3];

    let width = map.width as i32;
    let height = map.height as i32;
    let image = map.data.into_owned();
    let data: Vec<u8> = image.into_iter().step_by(4).collect();
    let data: Vec<f32> = data
        .iter()
        .map(|x| (1.0 - (*x as f32) / 255.0) * 10.0)
        .collect();
    let sceneWidth = r - l;
    let sceneLength = u - b;

    let xToWorld = |x: i32| ((x as f32) / (width as f32 - 1.0) * 1.04 - 0.02) * sceneWidth + l;

    let xFromWorld = |x: f32| {
        (((x - l) / sceneWidth + 0.02) / 1.04 * (width as f32 - 1.0)) as i32;
    };

    let yToWorld = |y: i32| ((y as f32) / (height as f32 - 1.0) * 1.04 - 0.02) * sceneLength + b;

    let yFromWorld = |y: f32| {
        (((y - b) / sceneLength + 0.02) / 1.04 * (height as f32 - 1.0)) as i32;
    };
    let g = |px: i32, py: i32| data[(py * width + px) as usize];

    let mut result = vec![];

    /*for u in 0..50 {
        let rev = u%2==0;
        let mk = 50;
        for v in 1..mk {
            let val = if rev { mk-v} else {v};
            let q = gatling.eval_dist(
                (gatling.u*u) as f32 / 100.0f32,
                (gatling.v*val) as f32 / 100.0f32, toolRadius);
            result.push(q);
        }
    }*/
    //for pt in &d.curves[1] {
        //result.push(wings.eval_dist(pt[0],pt[1], toolRadius));

    //}
    //
    result.extend(generate_gatling_details(gatling, &d.curves[0], toolRadius));


    for elem in &mut result {
            let tmp = *elem;

            //*elem = Vector3::new(tmp.x(), tmp.z(), f32::max(floorOffset - tmp.y()- toolRadius, floorOffset));
            *elem = Vector3::new(tmp.x(), tmp.y()+4.0, tmp.z());
            if elem.z() < 0.0{
            }
    }
    result
}

pub fn generate_contour(
    l: f32,
    r: f32,
    b: f32,
    u: f32,
    floorOffset: f32,
    materialHeight: f32,
    toolRadius: f32,
    start: (f32, f32, f32),
    map: glium::texture::RawImage2d<u8>,
) -> Vec<Vector3<f32>> {
    let width = map.width as i32;
    let height = map.height as i32;
    let image = map.data.into_owned();
    let data: Vec<u8> = image.into_iter().step_by(4).collect();
    let data: Vec<f32> = data
        .iter()
        .map(|x| (1.0 - (*x as f32) / 255.0) * 10.0)
        .collect();
    let sceneWidth = r - l;
    let sceneLength = u - b;

    let xToWorld = |x: i32| ((x as f32) / (width as f32 - 1.0) * 1.04 - 0.02) * sceneWidth + l;

    let yToWorld = |y: i32| ((y as f32) / (height as f32 - 1.0) * 1.04 - 0.02) * sceneLength + b;
    let mut y: i32 = 0;
    let g = |px: i32, py: i32| data[(py * width + px) as usize];

    let g2 = |px: i32, py: i32| data[(px * width + py) as usize];
    let h = g(0, 0); // to jest docelowa wartosc wszedzie gdzie wchodzimy

    let mut result = vec![];

    result.push(Vector3::new(start.0, start.1, start.2));

    let mut x: i32 = 35;
    let mut y: i32 = (height as f32 * 0.9) as i32;

    result.push(Vector3::new(xToWorld(x), yToWorld(y), materialHeight + 0.1));
    result.push(Vector3::new(xToWorld(x), yToWorld(y), floorOffset));

    let mut up = false;
    let step = 1835;
    move_tool(
        &mut result,
        x,
        -1,
        &mut y,
        h,
        &g,
        floorOffset,
        &mut up,
        &yToWorld,
        &xToWorld,
        height,
        width,
        step,
        false,
    );
    x += step as i32;

    let step = 500;
    up = false;
    move_tool(
        &mut result,
        y,
        100000,
        &mut x,
        h,
        &g2,
        floorOffset,
        &mut up,
        &xToWorld,
        &yToWorld,
        width,
        height,
        step,
        true,
    );
    y -= step as i32;

    let step = 550;
    up = true;
    move_tool(
        &mut result,
        x,
        1000000,
        &mut y,
        h,
        &g,
        floorOffset,
        &mut up,
        &yToWorld,
        &xToWorld,
        height,
        width,
        step,
        false,
    );
    x -= step as i32;

    let step = 764;
    up = false;
    move_tool(
        &mut result,
        y,
        100000,
        &mut x,
        h,
        &g2,
        floorOffset,
        &mut up,
        &xToWorld,
        &yToWorld,
        width,
        height,
        step,
        true,
    );
    y -= step as i32;

    let step = 300;
    up = true;
    move_tool(
        &mut result,
        x,
        1000000,
        &mut y,
        h,
        &g,
        floorOffset,
        &mut up,
        &yToWorld,
        &xToWorld,
        height,
        width,
        step,
        false,
    );
    x -= step as i32;

    let step = 750;
    up = true;
    move_tool(
        &mut result,
        y,
        -1,
        &mut x,
        h,
        &g2,
        floorOffset,
        &mut up,
        &xToWorld,
        &yToWorld,
        width,
        height,
        step,
        true,
    );
    y += step as i32;

    let step = 460;
    up = true;
    move_tool(
        &mut result,
        x,
        1000000,
        &mut y,
        h,
        &g,
        floorOffset,
        &mut up,
        &yToWorld,
        &xToWorld,
        height,
        width,
        step,
        false,
    );
    x -= step as i32;

    let step = 530;
    up = true;
    move_tool(
        &mut result,
        y,
        -1,
        &mut x,
        h,
        &g2,
        floorOffset,
        &mut up,
        &xToWorld,
        &yToWorld,
        width,
        height,
        step,
        true,
    );
    y += step as i32;

    result.push(Vector3::new(
        xToWorld(25),
        yToWorld((height as f32 * 0.9f32) as i32),
        materialHeight + 0.1,
    ));
    result.push(Vector3::new(start.0, start.1, start.2));
    result
}

pub fn generate_flat(
    l: f32,
    r: f32,
    b: f32,
    u: f32,
    floorOffset: f32,
    materialHeight: f32,
    toolRadius: f32,
    start: (f32, f32, f32),
    map: glium::texture::RawImage2d<u8>,
) -> Vec<Vector3<f32>> {
    let width = map.width as i32;
    let height = map.height as i32;
    let image = map.data.into_owned();
    let data: Vec<u8> = image.into_iter().step_by(4).collect();
    let data: Vec<f32> = data
        .iter()
        .map(|x| (1.0 - (*x as f32) / 255.0) * 10.0)
        .collect();
    let sceneWidth = r - l;
    let sceneLength = u - b;

    let xToWorld = |x: i32| ((x as f32) / (width as f32 - 1.0) * 1.04 - 0.02) * sceneWidth + l;

    let yToWorld = |y: i32| ((y as f32) / (height as f32 - 1.0) * 1.04 - 0.02) * sceneLength + b;

    let rows = ((r - l) / (1.8 * toolRadius)).ceil() as u32;
    let step = (map.width as f32 / rows as f32).floor() as u32;

    println!("will make {} rows with step {}", rows, step);

    let mut result = vec![];
    result.push(Vector3::new(start.0, start.1, start.2));
    result.push(Vector3::new(-8.5, -8.5, materialHeight + 0.1));
    result.push(Vector3::new(-8.5, -8.5, floorOffset));

    /*println!("x0 = {}", xToWorld(0));
    println!("x0 = {}", xToWorld((width-1) as u32));
*/
    // tutaj generowanie plaskich sciezek

    let mut up = true;
    let mut y: i32 = 0;
    let g = |px: i32, py: i32| data[(py * width + px) as usize];
    let h = g(0, 0); // to jest docelowa wartosc wszedzie gdzie wchodzimy

    let mut prev: i32 = -1;
    for i in 0..(rows + 1) {
        move_tool(
            &mut result,
            (i * step+5) as i32,
            prev * (step as i32)+5,
            &mut y,
            h,
            &g,
            floorOffset,
            &mut up,
            &yToWorld,
            &xToWorld,
            height,
            width,
            step,
            false,
        );
        prev = i as i32;
    }

    for i in (0..rows).rev() {
        move_tool(
            &mut result,
            (i * step+5) as i32,
            prev * (step as i32)+5,
            &mut y,
            h,
            &g,
            floorOffset,
            &mut up,
            &yToWorld,
            &xToWorld,
            height,
            width,
            step,
            false,
        );
        prev = i as i32;
    }

    let p = *result.last().unwrap();
    result.push(Vector3::new(p.x(), p.y(), materialHeight + 1.0));
    result.push(Vector3::new(start.0, start.1, start.2));
    println!("generated {} points", result.len());

    result
}

pub fn generate_rough(
    l: f32,
    r: f32,
    b: f32,
    u: f32,
    floorOffset: f32,
    materialHeight: f32,
    toolRadius: f32,
    start: (f32, f32, f32),
    map: glium::texture::RawImage2d<u8>,
) -> Vec<Vector3<f32>> {
    let width = map.width;
    let height = map.height;
    let image = map.data.into_owned();
    let data: Vec<u8> = image.into_iter().step_by(4).collect();
    let data: Vec<f32> = data
        .iter()
        .map(|x| (1.0 - (*x as f32) / 255.0) * 10.0)
        .collect();
    let sceneWidth = r - l;
    let sceneLength = u - b;
    println!("first: {}", data[0]);

    let xToWorld = |x: u32| (x as f32 / width as f32) * sceneWidth + l;

    let yToWorld = |y: u32| (y as f32 / height as f32) * sceneLength + b;

    let rows = ((r - l) / (1.1 * toolRadius)).ceil() as u32;
    let step = (map.width as f32 / rows as f32).floor() as u32;
    println!("generating {} rows", rows);

    let mut result = vec![];
    let mut upperResult = vec![];
    upperResult.push(Vector3::new(start.0, start.1, start.2));
    upperResult.push(Vector3::new(-8.0, -8.0, 6.0));
    result.push(Vector3::new(-8.0, -8.0, 6.0));

    // generate from starto to (l,b) here
    // dolna warstwa
    for i in 0..(rows + 1) {
        let mut x = (i * step);
        if x >= width {
            x = width - 1;
        }
        let x = x;
        let xScene = (i * step) as f32 / (width as f32) * sceneWidth + l;
        println!("generating row {} at {} pixel with X: {}", i, x, xScene);

        let mut points = vec![];
        let mut upperPoints = vec![];
        let mut prev = 0;

        let g = |y: u32| data[(y * width + x) as usize];
        let upper_g = |y: u32| f32::max(2.3, data[(y * width + x) as usize]);
        let mut prevPt = g(0);
        let mut upperPrev = upper_g(0);

        // starting point of pass
        points.push((x, 0));
        upperPoints.push((x, 0));

        for j in (10..height).step_by(6) {
            let h = g(j);
            let h2 = upper_g(j);

            if (h != prevPt) {
                points.push((x, j));
            }

            if (h2 != upperPrev) {
                upperPoints.push((x, j));
            }

            prev = j;
            prevPt = h;
            upperPrev = h2;
        }
        points.push((x, height - 1));
        upperPoints.push((x, height - 1));

        if i % 2 == 0 {
            result.extend(points.iter().map(|pt| {
                Vector3::<f32>::new(
                    xToWorld(pt.0),
                    yToWorld(pt.1),
                    floorOffset + g(pt.1) - toolRadius,
                )
            }));
            upperResult.extend(upperPoints.iter().map(|pt| {
                Vector3::<f32>::new(
                    xToWorld(pt.0),
                    yToWorld(pt.1),
                    floorOffset + upper_g(pt.1) - toolRadius,
                )
            }));
        } else {
            result.extend(points.iter().rev().map(|pt| {
                Vector3::<f32>::new(
                    xToWorld(pt.0),
                    yToWorld(pt.1),
                    floorOffset + g(pt.1) - toolRadius,
                )
            }));
            upperResult.extend(upperPoints.iter().rev().map(|pt| {
                Vector3::<f32>::new(
                    xToWorld(pt.0),
                    yToWorld(pt.1),
                    floorOffset + upper_g(pt.1) - toolRadius,
                )
            }));
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
