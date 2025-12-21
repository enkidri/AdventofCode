use std::fs;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {
    let test_path : &str = "src/test.txt";
    let input_path : &str = "src/input.txt";
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

// Used to read text file line by line
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where P: AsRef<Path>, {
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
fn part_one(file_path : &str) {
    // Parse the problem
    let mut values : Vec<i64> = Vec::new();
    if let Ok(lines) = read_lines(file_path) {
        // Consumes the iterator, returns an (Optional) String
        for line in lines.map_while(Result::ok) {
            values.push(line.parse::<i64>().unwrap());
        }
    }

    // Solve the problem
    let mut answer : i64 = 0;
    let mut is_found : bool = false;
    for i in 0..values.len() {
        for j in i+1..values.len() {
            if values.get(i).unwrap() + values.get(j).unwrap() == 2020 {
                answer = values.get(i).unwrap() * values.get(j).unwrap();
                is_found = true;
                break;
            }
        }

        if is_found {
            break;
        }
    }

    println!("Answer: {}", answer);
}

fn part_two(file_path : &str) {
    // Parse the problem
    let mut values : Vec<i64> = Vec::new();
    if let Ok(lines) = read_lines(file_path) {
        // Consumes the iterator, returns an (Optional) String
        for line in lines.map_while(Result::ok) {
            values.push(line.parse::<i64>().unwrap());
        }
    }

    // Solve the problem
    let mut answer : i64 = 0;
    let mut is_found : bool = false;
    for i in 0..values.len() {
        for j in i+1..values.len() {
            for k in j+1..values.len() {
                if values.get(i).unwrap() + values.get(j).unwrap() + 
                    values.get(k).unwrap() == 2020 {
                    answer = values.get(i).unwrap() * values.get(j).unwrap() *
                        values.get(k).unwrap();
                    is_found = true;
                    break;
                }
            }
            if is_found {
                break;
            }
        }

        if is_found {
            break;
        }
    }

    println!("Answer: {}", answer);
}
