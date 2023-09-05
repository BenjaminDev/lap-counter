// @generated automatically by Diesel CLI.

diesel::table! {
    laps (id) {
        id -> Text,
        race_id -> Text,
        driver_name -> Text,
        car_id -> Text,
        lap_time -> Timestamp,
    }
}
