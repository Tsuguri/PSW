use Data;
use Math::Vector::Vector2;
use Math::Vector::Vector3;

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


    // filter points
    
    let pts: Vec<Vector3<f32>> = points
            .iter()
            .map(|pt| Vector3::<f32>::new(xToWorld(pt.0), yToWorld(pt.1), floorOffset + h)).collect();

    let mut prev = pts.first().unwrap().clone(); 
    let last = pts.last().unwrap().clone();

    let mut ptss = vec![];
    ptss.push(prev);
    for pt in pts {
        if (pt-prev).len_squared() > 0.09 {
            prev = pt;
            ptss.push(pt);
        }

    }


    ptss.push(last);
    result.extend(ptss);
}

fn generate_bool_map(
    uRes: i32,
    vRes: i32,
    result: &mut [bool],
    u: u32,
    v: u32,
    path: &[Vector2<f32>],
    inv: Option<bool>,
) -> Vec<i32>{
    if (uRes * vRes) as usize != result.len() {
        panic!("wut!");
    }

    let mut data = vec![0i32; result.len()];

    {
        let mut set = |x: i32, y: i32, val: i32| {
            data[(x * vRes + y) as usize] += val;
        };

        for i in 0..(path.len() - 1) {
            let from = &path[i];
            let to = &path[i + 1];

            let up = from[0] > to[0];

            let fu = from[0] / (u as f32) * uRes as f32;
            let tu = to[0] / (u as f32) * uRes as f32;

            let pfu = f32::ceil(fu) as i32;
            let ptu = f32::ceil(tu) as i32;
            //let pfu = (if !up { f32::floor(fu)} else { f32::ceil(fu)}) as i32;
            //let ptu = (if !up { f32::floor(tu)} else { f32::ceil(tu)}) as i32;
            //println!("fu: {}, tu: {}, pfu: {}, ptu: {}", fu, tu, pfu, ptu);

            if pfu == ptu {
                continue;
            }

            if (from[0] - to[0]).abs() > 0.8 {
                let i = 0i32;
                let h = (from[1] + to[1]) / 2.0 -0.5;
                let ph = h / (v as f32) * vRes as f32;
                //println!("-----from: {}, to: {}, up to: {}", pfu, ptu, ph);
                for j in 0..vRes {
                    if (j as f32) < ph {
                        set(i, j, if !up { 1 } else { -1 });
                    } else {
                        set(i, j, if !up { -1 } else { 1 });
                    }
                }
                continue;
            }

            let f = if up { ptu } else { pfu };
            let t = if up { pfu } else { ptu };
            //println!("from: {}, to: {}", f, t);
            for i in (f..t) {
                let ru = (i as f32) / (uRes as f32) * (u as f32);
                let inter = (ru - from[0]) / (to[0] - from[0]);
                let rv = (to[1] - from[1]) * inter + from[1] - 0.5;
                let pv = rv / (v as f32) * vRes as f32;
                for j in 0..vRes {
                    if (j as f32) < pv {
                        set(i, j, if up { 1 } else { -1 });
                    } else {
                        set(i, j, if up { -1 } else { 1 });
                    }
                }
            }
        }
    }


    let cl = select(inv);


    for i in 0..result.len() {
        if cl(data[i]) {
            result[i] = false;
        }
    }
    data
}

fn select(inv: Option<bool>) -> Box<Fn(i32) -> bool> {

    match inv {
        Option::None => Box::new(|x| x==0),
        Option::Some(f) => match f {
            true => Box::new(|x| x>0),
            false => Box::new(|x| x<0),
        }
    }
}

fn generate_height_map(
    result: &mut [bool],
    u: i32,
    v: i32,
    surface: &Data::Surface,
    toolRadius: f32,
) {
    for up in 0..u {
        for vp in 0..v {
            let uc = (surface.u as i32 * up) as f32 / (u as f32);
            let vc = (surface.v as i32 * vp) as f32 / (v as f32);
            let q = surface.eval_dist(uc, vc, toolRadius);
            if q.y() < toolRadius {
                result[(up * v + vp) as usize] = false;
            }
        }
    }
}

fn generate_generic_details(
    surface: &Data::Surface,
    uRes: i32,
    vRes: i32,
    map: &[bool],
    toolRadius: f32,
    us: impl Iterator<Item = i32>,
    f: i32,
    t: i32,
    up_by: f32,
) -> Vec<Vector3<f32>> {
    let value = |up: i32, vp: i32| map[(up * vRes + vp) as usize];
    let mut result = vec![];

    for u in us {
        let rev = u % 2 == 0;
        for v in 1..(t - f) {
            let val = if rev { t - v } else { f + v };
            if !value(u, val) {
                continue;
            }
            let uc = (surface.u as i32 * u) as f32 / (uRes as f32);
            let vc = (surface.v as i32 * val) as f32 / (vRes as f32);
            //println!("uv: {}, uv: {}", uc, vc);
            let q = surface.eval_dist(uc, vc, toolRadius);
            result.push(q);
        }
    }

    // add up if far enough
    let mut res2 = Vec::with_capacity(result.len());

    if result.len() > 0 {
        for i in 0..(result.len() - 1) {
            let from = &result[i];
            let to = &result[i + 1];

            res2.push(*from);

            if (*to - *from).len_squared() > 0.08 {
                res2.push(Vector3::new(from.x(), from.y() + up_by, from.z()));
                res2.push(Vector3::new(to.x(), to.y() + up_by, to.z()));
            }

            res2.push(*to);
        }
    }

    res2
}

fn generate_gatling_details(
    gatling: &Data::Surface,
    gatling_wing: &[Vector2<f32>],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 40;
    let vRes: i32 = 60;

    let mut map = vec![true; (uRes * vRes) as usize];

    generate_height_map(&mut map, uRes, vRes, gatling, toolRadius);
    generate_bool_map(
        uRes,
        vRes,
        &mut map,
        gatling.u,
        gatling.v,
        gatling_wing,
        Some(true),
    );

    generate_generic_details(
        gatling,
        uRes,
        vRes,
        &map,
        toolRadius,
        0..uRes,
        0,
        vRes / 2,
        0.5,
    )
}

fn generate_cockpit_details(
    cockpit: &Data::Surface,
    from_cockpit: &[Vector2<f32>],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 60;
    let vRes: i32 = 120;

    let mut map = vec![true; (uRes * vRes) as usize];

    generate_bool_map(
        uRes,
        vRes,
        &mut map,
        cockpit.u,
        cockpit.v,
        from_cockpit,
        None,
    );

    generate_generic_details(
        cockpit,
        uRes,
        vRes,
        &map,
        toolRadius,
        ((uRes * 3 / 4)..uRes).chain(0..uRes / 4),
        3,
        vRes / 2,
        0.5,
    )
}

fn generate_fin_details(
    cockpit: &Data::Surface,
    constraints: [&[Vector2<f32>];2],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 80;
    let vRes: i32 = 120;

    let mut map = vec![true; (uRes * vRes) as usize];

    generate_bool_map(uRes, vRes, &mut map, cockpit.u, cockpit.v, constraints[0], Some(false));
    generate_bool_map(uRes, vRes, &mut map, cockpit.u, cockpit.v, constraints[1], Some(false));

    generate_generic_details(
        cockpit,
        uRes,
        vRes,
        &map,
        toolRadius,
        ((uRes / 2+10)..uRes).chain(0..uRes / 3-7),
        vRes / 2,
        vRes-4,
        1.0,
    )
}

fn generate_left_wing_details(
    wing: &Data::Surface,
    constraints: [&[Vector2<f32>]; 3],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 120;
    let vRes: i32 = 180;

    let mut map = vec![true; (uRes * vRes) as usize];


    generate_height_map(&mut map, uRes, vRes, wing, toolRadius);
    for constr in constraints.iter() {

        let dat = generate_bool_map(uRes, vRes, &mut map, wing.u, wing.v, constr, Some(false));
    }
    generate_generic_details(wing, uRes, vRes, &map, toolRadius, ((uRes/2)..uRes), 0, vRes/2, 0.5)
}

fn generate_right_wing_details(
    wing: &Data::Surface,
    constraints: [&[Vector2<f32>]; 2],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 120;
    let vRes: i32 = 180;

    let mut map = vec![true; (uRes * vRes) as usize];


    generate_height_map(&mut map, uRes, vRes, wing, toolRadius);
    for constr in constraints.iter() {

        let dat = generate_bool_map(uRes, vRes, &mut map, wing.u, wing.v, constr, Some(false));
    }
    generate_generic_details(wing, uRes, vRes, &map, toolRadius, ((uRes/2)..uRes), vRes/2, vRes, 0.4)
}

fn generate_hull_details(
    hull: &Data::Surface,
    constraints: [&[Vector2<f32>]; 3],
    toolRadius: f32,
) -> Vec<Vector3<f32>> {
    let uRes: i32 = 100;
    let vRes: i32 = 180;

    let mut map = vec![true; (uRes * vRes) as usize];

    generate_height_map(&mut map, uRes, vRes, hull, toolRadius);

    for constr in constraints.iter() {
        generate_bool_map(uRes, vRes, &mut map, hull.u, hull.v, constr, Some(true));
    }
    generate_generic_details(
        hull,
        uRes,
        vRes,
        &map,
        toolRadius,
        (0..(uRes / 2)).rev().chain(((uRes / 2)..uRes).rev()),
        vRes / 6,
        vRes * 77 / 100,
        1.0,
    )
}

fn generate_front_details(
    hull: &Data::Surface,
    toolRadius: f32
    )->Vec<Vector3<f32>> {

    let uRes: i32 = 60;
    let vRes: i32 = 120;

    let mut map = vec![true; (uRes * vRes) as usize];

    generate_height_map(&mut map, uRes, vRes, hull, toolRadius);

    generate_generic_details(
        hull,
        uRes,
        vRes,
        &map,
        toolRadius,
        (0..(uRes / 2)).rev().chain(((uRes / 2)..uRes).rev()),
        vRes/20,
        vRes/7,
        1.0,
    )
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


    result.push(Vector3::new(start.0, start.2, start.1));
    result.push(Vector3::new(1.0, 5.0, 8.0));
    result.push(Vector3::new(1.0, floorOffset + 0.5, 8.0));
    // curve: 5 albo 6

    
    result.extend(generate_left_wing_details(
        wings,
        [&d.curves[1], &d.curves[3], &d.curves[9]],
        toolRadius,
    ));
    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));
    result.push(Vector3::new(-7.0, 5.0, 6.0));

    result.extend(generate_right_wing_details(
        wings,
        [&d.curves[3], &d.curves[9]],
        toolRadius,
    ));
    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(-2.0, 5.0, -6.0));
    result.push(Vector3::new(-2.0, 0.5, -6.0));

    result.extend(generate_front_details(body,toolRadius));

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(-2.0, 5.0, -6.0));
    result.push(Vector3::new(-2.0, 0.5, -6.0));

    result.extend(generate_hull_details(body, [&d.curves[6], &d.curves[4], &d.curves[2]],toolRadius));

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(1.0, 5.0, -4.0));
    result.push(Vector3::new(1.0, 4.0, -4.0));
    result.extend(generate_cockpit_details(cockpit, &d.curves[5], toolRadius));

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(1.0, 5.0, 8.0));
    result.push(Vector3::new(1.0, 3.5, 8.0));
    result.extend(generate_fin_details(cockpit, [&d.curves[8], &d.curves[7]], toolRadius));

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(6.0, 5.0, 0.0));
    result.push(Vector3::new(6.0, 0.2, 0.0));
    result.extend(generate_gatling_details(gatling, &d.curves[0], toolRadius));

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    result.push(Vector3::new(4.8, 1.0, 1.5));

    // gatling-skrzydlo
    let len = d.curves[0].len();
    let cur = &d.curves[0];
    for pt in (len*34/100)..(len*13/20) {
        result.push(gatling.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
    }

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 3.0, lst.z()));

    result.push(Vector3::new(0.0, 5.0, -1.0));
    result.push(Vector3::new(0.0, 3.0, -1.0));
    // kokpit - kadłub
    let len = d.curves[4].len();
    let cur = &d.curves[4];

    for pt in (0)..(len) {
        result.push(body.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
    }
    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 3.0, lst.z()));

    result.push(Vector3::new(3.0, 3.0, -0.5));

    // skrzydlo - kadlub - fin - kadlub - prawe skrzydlo
    
    let cur = &d.curves[2];
    let len = cur.len();
    // lewe skrzydlo -> kadlub
    for pt in ((len*99/100)..(len)).chain((0)..(len*106/1000)) {
        result.push(body.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
        
    }

    let cur = &d.curves[6];
    let len = cur.len();
    for pt in ((len*95/100)..(len)).chain((0)..(len/3)) {
        result.push(body.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
    }

    let cur = &d.curves[2];
    let len = cur.len();
    for pt in (len*185/1000)..(len*305/1000) {
        result.push(body.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
    }

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 3.0, lst.z()));
    result.push(Vector3::new(3.0, 3.0, 1.0));

    // up bo koniec dużego szwu
    // teraz skrzydla-kokpit

    let cur = &d.curves[9];
    let len = cur.len();
    for pt in (len*2/100)..(len*31/100) {
        result.push(wings.eval_dist(cur[pt][0], cur[pt][1]-0.5, toolRadius));
    }

    let lst = result.last().unwrap().clone();
    result.push(Vector3::new(lst.x(), 5.0, lst.z()));

    let flag = true;

    for elem in &mut result {
        let tmp = *elem;
        if flag {
        *elem = Vector3::new(
            tmp.x(),
            tmp.z(),
            f32::max(floorOffset + tmp.y() - toolRadius, floorOffset),
        );
        } else {
        *elem = Vector3::new(tmp.x(), tmp.y(), tmp.z());
        }
    }

    let mut prev = result.first().unwrap().clone(); 
    let last = result.last().unwrap().clone();

    let mut ptss = vec![];
    ptss.push(prev);
    for pt in result{
        if (pt-prev).len_squared() > 0.01 {
            prev = pt;
            ptss.push(pt);
        }

    }


    ptss.push(last);
    ptss
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

    let step = 460;
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
            (i * step + 5) as i32,
            prev * (step as i32) + 5,
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
            (i * step + 5) as i32,
            prev * (step as i32) + 5,
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


    let mut prev = upperResult.first().unwrap().clone(); 
    let last = upperResult.last().unwrap().clone();

    let mut ptss = vec![];
    ptss.push(prev);
    for pt in upperResult {
        if (pt-prev).len_squared() > 0.09 {
            prev = pt;
            ptss.push(pt);
        }

    }


    ptss.push(last);

    println!("Result size: {}", ptss.len());
    ptss
}
