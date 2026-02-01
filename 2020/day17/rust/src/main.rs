use std::collections::{HashMap, HashSet};
use std::fs;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test_path);
    print!("Input ");
    part_two(input_path);
}

struct Cube {
    x: i32,
    y: i32,
    z: i32,
    w: i32,
}

impl From<Cube> for u64 {
    fn from(c: Cube) -> u64 {
        ((c.x + 50) as u64)
            | (((c.y + 50) as u64) << 16)
            | (((c.z + 50) as u64) << 32)
            | (((c.w + 50) as u64) << 48)
    }
}

impl From<u64> for Cube {
    fn from(k: u64) -> Self {
        Cube {
            x: ((k & 0xFFFF) as i32) - 50,
            y: (((k >> 16) & 0xFFFF) as i32) - 50,
            z: (((k >> 32) & 0xFFFF) as i32) - 50,
            w: (((k >> 48) & 0xFFFF) as i32) - 50,
        }
    }
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Couldnt find file");
    let mut current_active_cube: HashSet<u64> = content
        .lines()
        .enumerate()
        .flat_map(|(y, line)| {
            line.chars()
                .enumerate()
                .filter(|(_, c)| *c == '#')
                .map(move |(x, _)| {
                    Cube {
                        x: x as i32,
                        y: y as i32,
                        z: 0 as i32,
                        w: 0 as i32,
                    }
                    .into()
                })
        })
        .collect();

    let n_cycles = 6;
    for _ in 0..n_cycles {
        let mut next_active_cube: HashSet<u64> = HashSet::new();
        next_active_cube.reserve(current_active_cube.len());
        let mut inactive_neighbour_count: HashMap<u64, i32> = HashMap::new();
        inactive_neighbour_count.reserve(current_active_cube.len() * 26);

        for &cube in &current_active_cube {
            let mut num_active: i32 = 0;
            let c: Cube = cube.into();
            for dx in -1..=1 {
                for dy in -1..=1 {
                    for dz in -1..=1 {
                        if dx == 0 && dy == 0 && dz == 0 {
                            continue;
                        }

                        let to_check = Cube {
                            x: c.x + dx,
                            y: c.y + dy,
                            z: c.z + dz,
                            w: 0,
                        }
                        .into();
                        if current_active_cube.contains(&to_check) {
                            num_active += 1;
                        } else {
                            *inactive_neighbour_count.entry(to_check).or_insert(0) += 1;
                        }
                    }
                }
            }

            if num_active == 2 || num_active == 3 {
                next_active_cube.insert(cube);
            }
        }

        for (coord, count) in inactive_neighbour_count {
            if count == 3 {
                next_active_cube.insert(coord);
            }
        }

        current_active_cube = next_active_cube;
    }

    let answer = current_active_cube.len();
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Couldnt find file");
    let mut current_active_cube: HashSet<u64> = content
        .lines()
        .enumerate()
        .flat_map(|(y, line)| {
            line.chars()
                .enumerate()
                .filter(|(_, c)| *c == '#')
                .map(move |(x, _)| {
                    Cube {
                        x: x as i32,
                        y: y as i32,
                        z: 0 as i32,
                        w: 0 as i32,
                    }
                    .into()
                })
        })
        .collect();

    let n_cycles = 6;
    for _ in 0..n_cycles {
        let mut next_active_cube: HashSet<u64> = HashSet::new();
        next_active_cube.reserve(current_active_cube.len());
        let mut inactive_neighbour_count: HashMap<u64, i32> = HashMap::new();
        inactive_neighbour_count.reserve(current_active_cube.len() * 26);

        for &cube in &current_active_cube {
            let mut num_active: i32 = 0;
            let c: Cube = cube.into();
            for dx in -1..=1 {
                for dy in -1..=1 {
                    for dz in -1..=1 {
                        for dw in -1..=1 {
                            if dx == 0 && dy == 0 && dz == 0 && dw == 0 {
                                continue;
                            }

                            let to_check = Cube {
                                x: c.x + dx,
                                y: c.y + dy,
                                z: c.z + dz,
                                w: c.w + dw,
                            }
                            .into();
                            if current_active_cube.contains(&to_check) {
                                num_active += 1;
                            } else {
                                *inactive_neighbour_count.entry(to_check).or_insert(0) += 1;
                            }
                        }
                    }
                }
            }

            if num_active == 2 || num_active == 3 {
                next_active_cube.insert(cube);
            }
        }

        for (coord, count) in inactive_neighbour_count {
            if count == 3 {
                next_active_cube.insert(coord);
            }
        }

        current_active_cube = next_active_cube;
    }

    let answer = current_active_cube.len();
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
