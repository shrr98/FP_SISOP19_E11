# FP_sisop_E11
FP Sisop daemon + thread
1. Fungsi runCrontab<br>
Fungsi ini untuk menjalankan crontab dan mengecek apakah terjadi modifikasi pada crontab.
2. Fungsi runAJob<br>
Fungsi ini akan dijalankan oleh thread dari setiap cronjob.
3. Fungsi startSchedule<br>
Fungsi ini untuk memulai schedule, yaitu membaca file crontab.data dan menyimpan cron job pada list cronJobs. Fungsi ini akan dipanggil saat pertama kali crontab dijalankan dan setiap terjadi perubahan pada crontab.data.
4. Fungsi runSchedule<br>
Fungsi ini untuk menjalankan schedule dengan membuat thread yang akan meng-handle setiap cron job dan menunggu untuk menit selanjutnya dengan meng-invoke sleep(60).
5. Fungsi modifiedChecker<br>
Fungsi ini berfungsi untuk mengecek apakah terjadi perubahan pada crontab.data setiap detiknya dengan command 'find' dan argument -mmin 0.0167. 0.0167 ini setara dengan 1 detik, yaitu (1/60).
6. Fungsi resetAll<br>
Aktifkan resetFlag sehingga thread scheduling dapat dipause dan mengosongkan list cronjob.
