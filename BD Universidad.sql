CREATE DATABASE IF NOT EXISTS universidad;
USE universidad;

CREATE TABLE Alumnos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100),
    apellido VARCHAR(100),
    carnet VARCHAR(20),
    email VARCHAR(100)
);

CREATE TABLE Cursos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100),
    creditos INT,
    descripcion VARCHAR(255)
);

CREATE TABLE Alumnos_Cursos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_alumno INT,
    id_curso INT,
    nota FLOAT,
    fecha_inscripcion DATE,
    FOREIGN KEY (id_alumno) REFERENCES Alumnos(id),
    FOREIGN KEY (id_curso) REFERENCES Cursos(id)
);