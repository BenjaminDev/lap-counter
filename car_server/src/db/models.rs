use super::schema::laps;
use serde::{Deserialize, Serialize};
use diesel::{Insertable, Queryable};


#[derive(Serialize, Queryable)]
pub struct Lap {
    pub id: String,
    pub race_id: String,
    pub driver_name: String,
    pub car_id: String,
    pub lap_time: chrono::NaiveDateTime,
}

#[derive(Insertable)]
#[table_name = "laps"]
pub struct NewLap<'a> {
    pub id: &'a str,
    pub race_id: &'a str,
    pub driver_name: &'a str,
    pub car_id: &'a str,
    pub lap_time: chrono::NaiveDateTime,
}   