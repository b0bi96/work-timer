### 1. Build Project
To build this app, open your terminal and run the following command in the project root directory:
```bash
g++ -std=c++17 MyApp.cpp src/*.cpp src/ui/*.cpp  -o build/app `wx-config --cxxflags --libs`
```
### 2. Open Project
Once the application is built, you can run it by navigating to the build directory and executing the following command:
```bash
./app
```

