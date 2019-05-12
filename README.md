# FP_sisop_E11
FP Sisop daemon + thread
1. Fungsi timer<br>
Fungsi ini berfungsi untuk mengecek crontab setiap waktu yang ditentukan.
2. Fungsi runCrontab<br>
Fungsi ini untuk menjalankan crontab dan mengecek apakah terjadi modifikasi pada crontab.
3. Fungsi runAJob<br>
Fungsi ini berfungsi untuk menscan perintah crontabnya mulai dari waktu hingga filenya.
4. Fungsi startSchedule<br>
Fungsi ini berfungsi untuk mengecek * pertama adalah menit, * kedua adalah jam, * ketiga adalah hari, * keempat adalah minggu, dan * kelima adalah bulan.
5. Fungsi runSchedule<br>
Fungsi ini untuk menjalankan schedule dan membuat thread tiap crontab.
6. Fungsi modifiedChecker<br>
Fungsi ini berfungsi untuk mengecek apakah terjadi perubahan pada crontab.
7. Fungsi resetAll<br>
Aktifkan resetFlag sehingga thread scheduling dapat dihentikan
