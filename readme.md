Requires: argp

```
brew install argp
```

Example usage:

// This is 6 d10s, Botches on 1, successes on 6
```
$ roll -b1 -s6 6d10 
0 1 6 9 1 8 
// botches: 2
// pre-cancelled out successes: 3
successes: 1
```

// 5d10s explode 10s
```
$ roll 5d10!10
// 1 0 0 0 2 (4 9)
```
