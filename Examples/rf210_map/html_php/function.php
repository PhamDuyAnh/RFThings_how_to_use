<?php
	function checkValidTime($t) {
		session_start();
		// Kiểm tra xem session đã được tạo trước đó chưa
		if(isset($_SESSION['last_executed'])) {
			// Lấy thời điểm hiện tại
			$current_time = time();

			// Tính khoảng thời gian giữa các lần thực thi
			$time_difference = $current_time - $_SESSION['last_executed'];

			// Kiểm tra nếu khoảng thời gian nhỏ hơn 30 giây, thông báo và kết thúc
			if($time_difference < $t) {
			    echo "Quá thường xuyên! Vui lòng thử lại sau.";
			    exit;
			}
		}
		// Lưu thời điểm thực thi hiện tại vào session
		$_SESSION['last_executed'] = time();
		return true;
	}
?>