package main

import (
    "fmt"
    "net/http"
)

func main () {
    start_server()
}

func start_server() error {
    http.HandleFunc("/", handle_index)
    http.HandleFunc("/index", handle_index)

    err := http.ListenAndServe(":8080", nil)
    if err!=nil {
        fmt.Println(err.Error())
        return err
    }
    return nil
}

func handle_index(res http.ResponseWriter, req *http.Request) {
    res.Write([]byte("Hello Go language"))
}

