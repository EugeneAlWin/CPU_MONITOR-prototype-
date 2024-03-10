use std::io::{Read, Write};
use std::thread::sleep;
use std::time::{Duration, SystemTime, UNIX_EPOCH};

use compound_duration::format_dhms;
use serialport::SerialPort;
use sysinfo::{CpuRefreshKind, Disks, MemoryRefreshKind, RefreshKind, System};

const ACK_MESSAGE: &str = "ACK";
const HELLO_MESSAGE: &str = "CPU_MONITOR";
const SWITCH_SCREEN_MESSAGE: &str = "SWITCH";

fn draw_cpu(port: &mut Box<dyn SerialPort>) {
    let mut s = System::new_with_specifics(
        RefreshKind::new()
            .with_cpu(CpuRefreshKind::everything())
            .with_memory(MemoryRefreshKind::everything()),
    );
    sleep(sysinfo::MINIMUM_CPU_UPDATE_INTERVAL);
    s.refresh_cpu();
    s.refresh_memory();
    port.write_all(
        format!(
            "CPU: {:5.1}%\r\nRAM: {:5.1}%\r\nSWP: {:5.1}%\r\n",
            s.global_cpu_info().cpu_usage(),
            ((s.used_memory() as f64 / s.total_memory() as f64) * 100.0) as f32,
            ((s.used_swap() as f64 / s.total_swap() as f64) * 100.0) as f32,
        )
        .as_bytes(),
    )
    .unwrap();
}
fn draw_boot_time(port: &mut Box<dyn SerialPort>) {
    port.write_all(
        format!(
            "Boot time:\r\n{}",
            format_dhms(
                SystemTime::now()
                    .duration_since(UNIX_EPOCH)
                    .unwrap()
                    .as_secs()
                    - System::boot_time()
            )
        )
        .as_bytes(),
    )
    .unwrap();
}

fn draw_disk(port: &mut Box<dyn SerialPort>) {
    let disks = Disks::new_with_refreshed_list();
    let mut output = String::new();

    for disk in &disks {
        output.push_str(
            format!(
                "{} {:.1}GB\r\n",
                disk.mount_point().to_str().unwrap().replace('\\', ""),
                (disk.available_space() as f64 / (1024 * 1024 * 1024) as f64)
            )
            .as_str(),
        );
    }
    let left_strs = 5 - disks.len();
    for _ in 1..left_strs {
        output.push_str("           \r\n");
    }
    port.write_all(output.as_bytes()).unwrap();
}

fn main() {
    let mut port = serialport::new("COM8", 9600)
        .open()
        .expect("Failed to open port");

    let mut current_screen = 0;
    loop {
        let mut serial_buf: Vec<u8> = vec![0; ACK_MESSAGE.len()];
        _ = port.write(HELLO_MESSAGE.as_bytes()).expect("Write failed!");
        _ = port.read(&mut serial_buf).expect("Read failed");
        if String::from_utf8(serial_buf.clone()).unwrap() != ACK_MESSAGE {
            port.flush().unwrap();
            sleep(Duration::from_secs(2));
            continue;
        }

        loop {
            let res = port.bytes_to_read().unwrap();
            if res > 0 {
                let mut serial_switch_message: Vec<u8> = vec![0; res as usize];
                port.read_exact(serial_switch_message.as_mut_slice())
                    .unwrap();

                if !(serial_switch_message.is_empty()
                    && String::from_utf8(serial_switch_message.clone()).unwrap()
                        == SWITCH_SCREEN_MESSAGE)
                {
                    current_screen = (current_screen + 1) % 3;

                    _ = port.write(ACK_MESSAGE.as_bytes()).unwrap();
                    sleep(Duration::from_millis(1200));

                    serial_switch_message.clear();
                }
            }
            match current_screen {
                1 => draw_disk(&mut port),
                2 => draw_boot_time(&mut port),
                0 | _ => draw_cpu(&mut port),
            }
            sleep(Duration::from_millis(1500));
        }
    }
}
