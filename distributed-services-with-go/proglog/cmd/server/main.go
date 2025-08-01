package main

import (
	"log"

	"github.com/sarems/side-projects/distributed-services-with-go/proglog/internal/server"
)

func main() {
	srv := server.NewHTTPServer(":8080")
	log.Fatal(srv.ListenAndServe())
}
