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

struct Instruction {
    dir: char,
    count: i64,
}

struct State {
    x: i64,
    y: i64,
    heading: i32,
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let instructions: Vec<Instruction> = fs::read_to_string(file_path)
        .expect("Couldt find file")
        .lines()
        .map(|l| {
            let dir = l.chars().next().unwrap();
            let count: i64 = l[1..].parse().expect("Couldnt parse");
            Instruction { dir, count }
        })
        .collect();

    // solve
    let initial_state = State {
        x: 0,
        y: 0,
        heading: 3,
    };
    let end_state: State = instructions.iter().fold(initial_state, |mut state, inst| {
        match inst.dir {
            'L' => state.heading = (state.heading - (inst.count / 90) as i32).rem_euclid(4),
            'R' => state.heading = (state.heading + (inst.count / 90) as i32).rem_euclid(4),
            'N' => state.y += inst.count,
            'S' => state.y -= inst.count,
            'E' => state.x += inst.count,
            'W' => state.x -= inst.count,
            'F' => match state.heading {
                0 => state.y -= inst.count,
                1 => state.x -= inst.count,
                2 => state.y += inst.count,
                3 => state.x += inst.count,
                _ => unreachable!(),
            },
            _ => {}
        }
        state
    });

    let answer = end_state.x.abs() + end_state.y.abs();
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
    let instructions: Vec<Instruction> = fs::read_to_string(file_path)
        .expect("Couldt find file")
        .lines()
        .map(|l| {
            let dir = l.chars().next().unwrap();
            let count: i64 = l[1..].parse().expect("Couldnt parse");
            Instruction { dir, count }
        })
        .collect();

    // solve
    let initial_state = State {
        x: 0,
        y: 0,
        heading: 3,
    };
    let initial_waypoint = State {
        x: 10,
        y: 1,
        heading: 0,
    };
    let (end_state, _) = instructions.iter().fold(
        (initial_state, initial_waypoint),
        |(mut state, mut wp), inst| {
            match inst.dir {
                'L' | 'R' => {
                    let num_turns = (inst.count / 90) as i32;
                    for _ in 0..num_turns {
                        if inst.dir == 'R' {
                            wp = State {
                                x: wp.y,
                                y: -wp.x,
                                heading: 0,
                            };
                        } else {
                            wp = State {
                                x: -wp.y,
                                y: wp.x,
                                heading: 0,
                            }
                        }
                    }
                }
                'N' => wp.y += inst.count,
                'S' => wp.y -= inst.count,
                'E' => wp.x += inst.count,
                'W' => wp.x -= inst.count,
                'F' => {
                    state.x += wp.x * inst.count;
                    state.y += wp.y * inst.count;
                }
                _ => {}
            }
            (state, wp)
        },
    );

    let answer = end_state.x.abs() + end_state.y.abs();
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
