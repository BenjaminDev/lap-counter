pub mod models;
pub mod schema;

use diesel::prelude::*;
use dotenv::dotenv;
use std::env;

use models::*;
use schema::laps;
use schema::laps::dsl::*;

fn establish_connection() -> SqliteConnection {
    dotenv().ok();

    let database_url = env::var("DATABASE_URL")
        .expect("DATABASE_URL must be set");
    SqliteConnection::establish(&database_url)
        .unwrap_or_else(|_| panic!("Error connecting to {}", 
                                   database_url))
}

pub fn get_all_laps_for(query_race_id: String) -> Vec<Lap> {
    let mut connection = establish_connection();
    laps
        .filter(race_id.eq(query_race_id))
        .order(lap_time.asc())
        .load::<Lap>(&mut connection)
        .expect("Error loading laps")
}

pub fn add_lap_entry(new_lap: NewLap) -> Lap {
    let mut connection = establish_connection();
    diesel::insert_into(laps::table)
        .values(new_lap)
        .execute(&mut connection)
        .expect("Error saving new post");

    laps.order(id.desc()).first(&mut connection).unwrap()
}