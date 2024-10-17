# Introduction 

First of all, CI (CImproved) was developed with the aim of being able to rewrite codebases written in C, very quickly and without wasting too much time, as might be the case with another programming language (Rust, Go, etc.). Here's an example of a graph showing how CI's rewriting cycle compares with that of another language:

CI:

```
------------------------- Rewrite software

------------------------- Add new features
```

Other language:

```
------------------------- Rewrite software

                         ------------------------- Add new features
```

How can this be possible? In fact, CI is so close to C that around 99% of its syntax is found in C. So, in effect, you can convert your codebase part by part to CI and still continue to develop new features, which clearly wouldn't be the case if a language like Rust were used. So, if you've been following along, if you're a C developer, you should be able to get to grips with this language very quickly.
