# Final Project Mata Kuliah Struktur Data dan Algoritma

Proyek merupakan project akhir mata kuliah struktur data dan Algoritma,Dosen Pengampu adalah "Arta Kusuma Hernanda,S.T.,M.T".Tugas dari project ini adalah membuat Collision Simulations menggunakan bahasa pemrograman C++

Nama : Vinsen Dwi Putra
NRP  : 5024241094

## Daftar isi 
- [Overview](#overview)
- [Dependencies](#dependencies)
- [Cara build dan run Project](#cara-build-dan-run-project)
- [Progress](#progress)

## Overview
Proyek ini dibuat untuk menampilkan simulasi interaksi dan tabrakan antara berbagai objek dengan menggunakan SFML (Simple and Fast Multimedia Library) sebagai alat untuk merender grafik. Melalui simulasi tersebut, berbagai konsep pada struktur data dan analisis algoritma diterapkan secara langsung, khususnya dalam mekanisme pendeteksian serta penanganan benturan antar objek menggunakan Fitur BruteForce dan Quadtree.

## Dependencies

- SFML 3.0.2
- C++17 

## Cara build dan run Project

Ikuti langkah berikut untuk Build dan run proyek:

1. Pastikan compiler yang mendukung C++17 dan library SFML sudah terpasang.
2. Clone repositori ini ke komputer Anda.
3. Gunakan perintah ini dalam terminal untuk build SFML and compile  projectnya:
   ```bash
   g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o main
   ```
4. Sesudah build dan compile projectnya, jalankan simulasinya:
   ```bash
   ./main
   ```

## Progress

- [x] Meyiapkan Library SFML dan proses buildnya.
- [x] Mengembangkan fitur memunculkan bola di awal dan menambahkan bola di pertengahan program
- [x] Menambahkan collision detection antar objek
- [x] Mengimplementasikan metode Brute Force dan Quadtree


27-September-2025

- Menyiapkan Library SFML dan Compiler.
- Beralih Ke WSL karena masalah terhadap prioritas compiler di windows yang tidak bisa membaca compiler serta library yang di butuhkan

28-September-2025

- Terdapat Code dasar untuk Test Functionality dari SFML
- Initial Commit pada github untuk Testing cara kerja github 

25-November-2025

- Object baru hanya satu buah dan cuma bisa tabrakan dengan dinding ujung windows

29-November-2025

- Object yang auto generated ketika program pertama kali dijalankan bisa di atur dan juga bisa di generate manual di lokasi cursor langsung saat program berjalan menggunakan "Left mouse button"

30-November-2025

-Penambahan Algoritma BruteFroce untuk Menangani Deteksi Collision dan Respons Tabrakan Pada partikel

1-Desember-2025

-Penambahan Algoritma QuadTree yang bekerja sama dengan Algoritma BruteForce dalam menangani collision yang bisa di Algoritma nya nanti bisa ditoggle mana yang akan bekerja
-Penambahan Check Counter untuk tahu berapa algoritma yang sedang bekerja

