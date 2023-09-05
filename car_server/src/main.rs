//! Run with
//!
//! ```not_rust
//! cargo run -p example-templates
//! ```

use askama::Template;
use axum::{
    extract,
    http::StatusCode,
    response::{Html, IntoResponse, Response},
    routing::get,
    routing::get_service,
    Json, Router,
};
use chrono::{self, DateTime};
use chrono::{TimeZone, Utc};
use diesel::sql_types::Timestamp;
use std::net::SocketAddr;
use std::{io, sync::Arc};
use tower_http::{
    services::{ServeDir, ServeFile},
    trace::TraceLayer,
};
use tracing_subscriber::{layer::SubscriberExt, util::SubscriberInitExt};
use uuid::{self, timestamp};
mod db;
use db::{models::NewLap, *};
const FRONT_PUBLIC: &str = "./frontend/carcounter/build";

// fn lap_counter_routes()->Router<>{
//     let app = Router::new()
//         .route("/add_lap/:car_id", get(add_lap))
//         .route("/all", get(show_all))
//         .merge(front_public_route());
// }

#[tokio::main]
async fn main() {
    tracing_subscriber::registry()
        .with(
            tracing_subscriber::EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| "example_templates=debug".into()),
        )
        .with(tracing_subscriber::fmt::layer())
        .init();

    // build our application with some routes
    // let app = Router::new().route("/greet/:name", get(greet));
    let app = Router::new()
        .route("/add_lap/:car_id/:timestamp_id", get(add_lap))
        .route("/all", get(show_all))
        .merge(front_public_route());
    // .merge(services::backend(session_layer, shared_state));

    // run it
    let addr = SocketAddr::from(([127, 0, 0, 1], 3000));
    tracing::debug!("listening on {}", addr);
    axum::Server::bind(&addr)
        .serve(app.into_make_service())
        .await
        .unwrap();
}

// fn car_routes()-> Router<dyn IntoResponse>{
//     Router::new()
//         .route("/add_lap/:car_id", get(add_lap))
//         .route("/all", get(show_all))
//         .merge(front_public_route());
// }
async fn set_current_driver(
    extract::P
)
async fn add_lap(
    // extract::Path(car_id): extract::Path<String>,
    extract::Path((car_id, timestamp_id)): extract::Path<(String, i64)>,
) -> impl IntoResponse {
    let id = uuid::Uuid::new_v4().to_string();
    let new_lap = NewLap {
        id: id.as_str(),
        race_id: "1",
        driver_name: "Matt",
        car_id: &car_id,
        lap_time: Utc.timestamp_opt(timestamp_id, 0).unwrap().naive_utc(),
        // lap_time:chrono::Utc::now().naive_utc(),
    };

    add_lap_entry(new_lap);
}

async fn show_all() -> impl IntoResponse {
    return Json(get_all_laps_for('1'.to_string()));
}
// async fn get_all_laps

// async fn handle_error(_err: io::Error) -> impl IntoResponse {
//     (
//         StatusCode::INTERNAL_SERVER_ERROR,
//         "Something went wrong accessing static files...",
//     )
// }
pub fn front_public_route() -> Router {
    Router::new()
        .fallback_service(get_service(ServeDir::new(FRONT_PUBLIC)))
        .layer(TraceLayer::new_for_http())
}

#[cfg(test)]
mod tests {
    use super::*;
    use axum::{body::Body, http::Request};
    use rstest::rstest;
    use serde_json::Value;
    use tower::ServiceExt;

    /// Helper function to create a GET request for a given URI.
    fn send_get_request(uri: &str) -> Request<Body> {
        Request::builder()
            .uri(uri)
            .method("GET")
            .body(Body::empty())
            .unwrap()
    }

    /// Test function for the successful execution of the get_heroes handler function.
    ///
    /// This test function uses the rstest crate to test the handler function with different
    /// query parameters. The test function is executed for each test case.
    #[rstest]
    #[case("/race?id=1")] // Verify that % is appended to the filter
    #[case("/?nam")] // Verify that % is not appended to the filter if it already ends with %
    #[case("/")] // Verify that % is used as the default filter
    #[tokio::test]
    async fn get_by_name_success(#[case] uri: &'static str) {
        // Create repository. Note that this would turn out test into an integration test
        // if we would use a real database here. We would need to prepare a well-defined
        // test database and clean it up after the test.
        // let repo: HeroesRepositoryState = Arc::new(HeroesRepository());

        // Create the app with the repository as state.
        let app = heroes_routes().with_state(repo);

        // Call the app with a GET request to the get_heroes endpoint.
        let response = app.oneshot(send_get_request(uri)).await.unwrap();

        // Check the response status code.
        assert_eq!(response.status(), StatusCode::OK);

        // Check the response body.
        let body = hyper::body::to_bytes(response.into_body()).await.unwrap();
        let body: Value = serde_json::from_slice(&body).unwrap();

        // Here we just verify that we get back an array. In real life, you would probably
        // verify the content of the array as well. To do this, you need the well-defined
        // test database mentioned above.
        assert!(matches!(body, Value::Array { .. }));
    }

    /// Test DB error handling in get_heroes handler function.
    #[tokio::test]
    async fn get_by_name_failure() {
        // Create repository. Note that this would turn out test into an integration test
        // if we would use a real database here. We would need to prepare a well-defined
        // test database and clean it up after the test.
        let repo: HeroesRepositoryState = Arc::new(HeroesRepository());

        // Create the app with the repository as state.
        let app = heroes_routes().with_state(repo);

        // Call the app with a GET request to the get_heroes endpoint. Note that we need
        // to specifiy a filter from which we know that no heroes will be returned.
        // To be able to do this, you need the well-defined test database mentioned above.
        let response = app
            .oneshot(send_get_request("/?name=Spider"))
            .await
            .unwrap();

        // Check the response status code.
        assert_eq!(response.status(), StatusCode::NOT_FOUND);
    }

    // Note that we cannot write a test simulating proper handling of technical
    // errors (e.g. DB crash) in our API handler. This is because we cannot mock
    // the DB access in our test.
}
