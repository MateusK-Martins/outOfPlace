use clap::{Command, Parser, Subcommand};

#[derive(Parser, Debug)]
#[command(name = "rfm")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand, Debug)]
enum Commands {
    AccentRemover,
}
fn main() {
    let cli = Cli::parse();

    match cli.command {
        Commands::AccentRemover => {
            
        },

        _ => {}
    }
}
