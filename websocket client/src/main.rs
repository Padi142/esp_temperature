use std::io::{stdout, Write};

use tungstenite::{connect};
use crossterm::{QueueableCommand, cursor, terminal, ExecutableCommand};
use chrono::{self, Utc};
use url::Url;
fn main() {   
    let mut stdout = stdout();
    
    stdout.execute(cursor::Hide).unwrap();
    // Connect to the WS server locally
    let (mut socket, _response) = connect(Url::parse("ws://192.168.0.105/ws").unwrap()).expect("Can't connect");    
    
    // Loop forever, handling parsing each message
    loop {
        let msg = socket.read_message().expect("Error reading message");
        let msg = match msg {
            tungstenite::Message::Text(s) => { s }
            _ => { panic!() }
        };
        let now = Utc::now();
        let time = now.format("%H:%M:%S");

        stdout.queue(cursor::SavePosition).unwrap();
        stdout.write_all(format!("{:?} - Last sinced on: {}", msg,time).as_bytes()).unwrap();
        stdout.queue(cursor::RestorePosition).unwrap();
        stdout.flush().unwrap();

        stdout.queue(cursor::RestorePosition).unwrap();
        stdout.queue(terminal::Clear(terminal::ClearType::FromCursorDown)).unwrap();
    }
}