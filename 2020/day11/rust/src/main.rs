use std::fs;
use std::io::{self, BufRead};
use std::ops::Add;
use std::path::Path;
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct Index {
    i: i64,
    j: i64,
}

impl Add for Index {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            i: self.i + other.i,
            j: self.j + other.j,
        }
    }
}

#[derive(Clone)]
struct Grid(Vec<Vec<char>>);

impl Grid {
    fn get(&self, idx: Index) -> Option<char> {
        if idx.i < 0 || idx.j < 0 {
            return None;
        }
        self.0.get(idx.i as usize)?.get(idx.j as usize).copied()
    }

    fn set(&mut self, idx: Index, val: char) {
        self.0[idx.i as usize][idx.j as usize] = val;
    }

    fn count_occupied(&self) -> usize {
        self.0.iter().flatten().filter(|&&c| c == '#').count()
    }
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let raw_grid: Vec<Vec<char>> = fs::read_to_string(file_path)
        .expect("File not found")
        .lines()
        .map(|l| l.chars().collect())
        .collect();

    let mut grid = Grid(raw_grid);
    let mut next_grid = grid.clone();

    let seat_indices: Vec<Index> = grid
        .0
        .iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.iter()
                .enumerate()
                .filter(|(_, c)| **c == 'L')
                .map(move |(j, _)| Index {
                    i: i as i64,
                    j: j as i64,
                })
        })
        .collect();
    let offsets: Vec<Index> = vec![
        Index { i: -1, j: -1 },
        Index { i: -1, j: 0 },
        Index { i: -1, j: 1 },
        Index { i: 0, j: -1 },
        Index { i: 0, j: 1 },
        Index { i: 1, j: -1 },
        Index { i: 1, j: 0 },
        Index { i: 1, j: 1 },
    ];

    let mut changed: bool = true;
    while changed {
        changed = false;

        for &idx in &seat_indices {
            let c = grid.get(idx).unwrap();

            let num_occupied = offsets
                .iter()
                .filter_map(|&off| grid.get(idx + off))
                .filter(|&c| c == '#')
                .count();

            let new_char = match (c, num_occupied) {
                ('L', 0) => {
                    changed = true;
                    '#'
                }
                ('#', n) if n >= 4 => {
                    changed = true;
                    'L'
                }
                (c, _) => c,
            };
            next_grid.set(idx, new_char);
        }
        std::mem::swap(&mut grid, &mut next_grid);
    }

    let answer = grid.count_occupied();

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

    // parse
    let raw_grid: Vec<Vec<char>> = fs::read_to_string(file_path)
        .expect("File not found")
        .lines()
        .map(|l| l.chars().collect())
        .collect();

    let mut grid = Grid(raw_grid);
    let mut next_grid = grid.clone();

    let seat_indices: Vec<Index> = grid
        .0
        .iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.iter()
                .enumerate()
                .filter(|(_, c)| **c == 'L')
                .map(move |(j, _)| Index {
                    i: i as i64,
                    j: j as i64,
                })
        })
        .collect();
    let offsets: Vec<Index> = vec![
        Index { i: -1, j: -1 },
        Index { i: -1, j: 0 },
        Index { i: -1, j: 1 },
        Index { i: 0, j: -1 },
        Index { i: 0, j: 1 },
        Index { i: 1, j: -1 },
        Index { i: 1, j: 0 },
        Index { i: 1, j: 1 },
    ];

    let mut changed: bool = true;
    while changed {
        changed = false;

        for &idx in &seat_indices {
            let c = grid.get(idx).unwrap();

            // count number of occupied chairs
            let mut num_occupied = 0;
            for &offset in &offsets {
                let mut current_idx = idx + offset;
                while let Some(to_check) = grid.get(current_idx) {
                    match to_check {
                        'L' => break,
                        '#' => {
                            num_occupied += 1;
                            break;
                        }
                        _ => current_idx = current_idx + offset,
                    }
                }
            }

            // update the state
            let new_char = match (c, num_occupied) {
                ('L', 0) => {
                    changed = true;
                    '#'
                }
                ('#', n) if n >= 5 => {
                    changed = true;
                    'L'
                }
                (c, _) => c,
            };
            next_grid.set(idx, new_char);
        }
        std::mem::swap(&mut grid, &mut next_grid);
    }

    let answer = grid.count_occupied();
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
