use std::io::{self, Write};
use std::process::Command;
use std::path::Path;

fn main() {
    println!("--- MircoOS Term v0.3 ---");

    let path_dirs = ["/bin", "/sbin", "/usr/bin", "/usr/sbin"];

    loop {
        print!("term> ");
        io::stdout().flush().expect("term: std error!");

        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("term: error while reading input!");

        let parts: Vec<&str> = input.trim().split_whitespace().collect();

        if parts.is_empty() {
            continue;
        }

        let cmd_name = parts[0];
        let args = &parts[1..];


        if cmd_name == "exit" {
            break;
        } else if cmd_name == "clear" {
            println!("\033[H\033[2J");
            continue;
        } else if cmd_name == "gtd" {
            if !args.is_empty() {
                if let Err(e) = std::env::set_current_dir(args[0]) {
                    println!("gtd: error: {}", e);
                }
            } else {
                println!("gtd: missing path");
            }
            continue;
        }


        let mut target_path = cmd_name.to_string();

        if !cmd_name.starts_with('/') {
            for dir in path_dirs {
                let p = format!("{}/{}", dir, cmd_name);
                if Path::new(&p).exists() {
                    target_path = p;
                    break;
                }
            }
        }

        let child = Command::new(&target_path)
            .args(args)
            .spawn();

        match child {
            Ok(mut process) => {
                process.wait().expect("term: command execution failed");
            }
            Err(_) => {
                println!("term: command '{}' not found.", cmd_name);
            }
        }
    }
}