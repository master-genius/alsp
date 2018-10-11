package main

import (
    "fmt"
    "time"
)

func fibo(f, q chan int) {
    for {
        select {
            case <- f:
                fmt.Println("** Bang **")
            case <- q:
                goto end_for
            default:
                fmt.Println("waiting..")
                time.Sleep(100 * time.Millisecond)
        }
    }
    end_for:
        fmt.Println("--END--")
}

func main () {
    f := make(chan int)
    q := make(chan int)
    go func() {
        for i:=0; i<5; i++ {
            time.Sleep(50*time.Millisecond)
            f <- i
            //fmt.Println(<-f)
        }
        q <- 1
    }()

    fibo(f , q)
}

