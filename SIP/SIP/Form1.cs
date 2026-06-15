using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace SIP
{
    public partial class Form1 : Form
    {
        // 將 SerialPort 宣告為全域變數
        private SerialPort serialPort;

        public Form1()
        {
            InitializeComponent();

            // 在建構子或 Form_Load 中初始化 SerialPort
            serialPort = new SerialPort("COM3", 9600);

            // 💡 【新增】綁定收到資料時的事件處理函式
            serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);

            // 設定視窗關閉時的事件，確保資源有被釋放
            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 視窗開啟時，嘗試連線一次即可
            try
            {
                if (!serialPort.IsOpen)
                {
                    serialPort.Open();
                    Console.WriteLine("成功連接到 Arduino！");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"無法開啟序列埠: {ex.Message}", "連線錯誤", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // 💡 【新增】當 Arduino 傳送資料過來時，會自動觸發這個方法
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                // 讀取來自 Arduino 的一行資料 (直到接收到 \n 換行符號)
                string incomingData = serialPort.ReadLine().Trim();

                // 💡 跨執行緒處理：因為 DataReceived 在背景執行，不能直接控制 label1，必須透過 Invoke
                this.Invoke(new Action(() =>
                {
                    // 在這裡處理收到的資料，例如顯示在 label2 上，或是根據資料改變 label1
                    // 這裡先範例：把收到的內容顯示在 label1 (或者您可以自己換成別的控制項)
                    label1.Text = $"{incomingData}按下重新鍵繼續";
                }));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"接收錯誤: {ex.Message}");
            }
        }

        // 封裝一個通用的傳送方法，減少重複程式碼
        private void SendSerialData(string data)
        {
            try
            {
                if (serialPort != null && serialPort.IsOpen)
                {
                    serialPort.Write(data);
                    Console.WriteLine($"已發送訊號 '{data}'");
                }
                else
                {
                    Console.WriteLine("錯誤：序列埠未開啟！");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"發送錯誤: {ex.Message}");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SendSerialData("1");
            label1.Text = "已暫停";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SendSerialData("0");
            label1.Text = "遊戲中...";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SendSerialData("2");
            label1.Text = "遊戲中...";
        }

        // 當使用者關閉視窗時，自動安全地關閉序列埠
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                // 💡 【調整】關閉前先取消訂閱事件，避免關閉過程中還在嘗試接收資料導致崩潰
                serialPort.DataReceived -= SerialPort_DataReceived;

                serialPort.Close();
                serialPort.Dispose(); // 徹底釋放資源
                Console.WriteLine("已關閉 Arduino 連線。");
            }
        }
    }
}