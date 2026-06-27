using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace SIP
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort;

        public Form1()
        {
            InitializeComponent();

            serialPort = new SerialPort("COM3", 9600);

            serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);

            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
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

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string incomingData = serialPort.ReadLine().Trim();

                this.Invoke(new Action(() =>
                {
                    label1.Text = $"{incomingData}按下重新鍵繼續";
                }));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"接收錯誤: {ex.Message}");
            }
        }

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

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.DataReceived -= SerialPort_DataReceived;

                serialPort.Close();
                serialPort.Dispose(); 
                Console.WriteLine("已關閉 Arduino 連線。");
            }
        }
    }
}