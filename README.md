# RFID-Access-Control-8051
Mô phỏng hệ thống kiểm soát ra vào
## 📂 Cấu trúc dự án
- **main.c**: Code xử lý trung tâm, điều phối hoạt động của hệ thống.
- **I2C_EEPROM.c/h**: Thư viện giao tiếp chuẩn I2C để đọc/ghi dữ liệu thẻ vào chip nhớ 24C04.
- **LCD.c/h**: Thư viện điều khiển màn hình LCD 16x2.
- **main.pdsprj**: File mô phỏng mạch nguyên lý trên phần mềm Proteus.

## 🛠 Linh kiện chính (Mô phỏng)
- **Vi điều khiển:** AT89S52 (Họ 8051).
- **Module RFID:** Sử dụng Virtual Terminal để nhập mã thẻ giả lập.
- **EEPROM 24C04:** Lưu trữ UID của thẻ Master và các thẻ thành viên (dữ liệu không bị mất khi tắt điện).
- **LCD 16x2:** Hiển thị trạng thái hệ thống và thông báo.
- **Relay/Led:** Hiện tại sử dụng LED để mô phỏng trạng thái đóng/ngắt của Relay khóa cửa.
- **Button:** Nút nhấn Exit (Mở cửa từ bên trong).

## ⚙️ Chức năng hệ thống
1. **Xác thực:** Đọc và so sánh mã thẻ từ Terminal với dữ liệu trong EEPROM.
2. **Quản lý thẻ:** Chức năng thêm thẻ mới bằng thẻ Master.
3. **Reset dữ liệu:** Xóa toàn bộ thẻ đã lưu (trừ thẻ Master).
4. **Hiển thị:** Thông báo trạng thái (Access Granted, Denied, Add Mode...) trên LCD.
5. **Điều khiển cửa:** Kích hoạt LED (mô phỏng Relay) khi thẻ hợp lệ hoặc nhấn nút Exit.

## 🧠 Nguyên lý hoạt động (Workflow)

### 1. Trạng thái ban đầu
- Khi khởi động, hệ thống chưa có thẻ thành viên nào được phép mở cửa.
- Hệ thống chờ tín hiệu từ đầu đọc thẻ hoặc nút nhấn.

### 2. Chế độ Thêm thẻ (Add Mode)
- **Bước 1:** Quẹt **Thẻ Master** lần 1.
- **Bước 2:** Hệ thống vào chế độ học lệnh. Quẹt **Thẻ mới** cần thêm.
- **Kết quả:** Thẻ mới được lưu vào EEPROM và có thể dùng để mở cửa.

### 3. Chế độ Mở cửa (User Mode)
- Quẹt thẻ đã được thêm vào hệ thống -> LED sáng (Cửa mở) -> Hiển thị "Access Granted".
- Quẹt thẻ chưa đăng ký -> Hiển thị "Access Denied".

### 4. Chế độ Reset (Xóa tất cả)
- Quẹt **Thẻ Master 2 lần liên tiếp**.
- Hệ thống sẽ xóa sạch danh sách thẻ thành viên, chỉ giữ lại thẻ Master.

### 5. Nút nhấn EXIT
- Nút nhấn dành cho người ở bên trong muốn đi ra.
- Khi nhấn nút: Cửa luôn mở (LED sáng) bất kể trạng thái thẻ.
