using UnityEngine;
using System.IO.Ports;

public class Arduino : MonoBehaviour {
    public Transform steer;

    public float wheelFactor = 1f;

    private string rawData = "";
    private int steerValue = 0;
    private int wheelValue = 0;
    private int ringValue = 0;



    // change your serial port
    SerialPort serialPort = new SerialPort("COM5", 9600);
    
    // Start is called before the first frame update
    void Start() {
        serialPort.Open();
        serialPort.ReadTimeout = 100; // In my case, 100 was a good amount to allow quite smooth transition. 
    }

    // Update is called once per frame
    void Update() {
        if (serialPort.IsOpen) {
            try {
                //Debug.Log(serialPort.ReadLine());
                rawData = serialPort.ReadLine();
                
                if (rawData.Length > 0) {
                    steerValue = int.Parse(rawData.Substring(0, 2));
                    wheelValue = int.Parse(rawData.Substring(2, 1));
                    ringValue = int.Parse(rawData.Substring(3, 1));

                    //Debug.Log("Rotation: " + steerValue + ", Speed: " + wheelValue + ", Ring: " + ringValue);

                    // Rotation
                    float rotation = steerValue - 15f;
                    rotation = rotation * 15f;
                    Debug.Log("Rotation: " + rotation);
                    transform.localEulerAngles = new Vector3(0f, rotation, 0f);

                    // Speed
                    float speed = wheelValue * wheelFactor;
                    transform.Translate(Vector3.forward * speed * Time.deltaTime);

                    // Ring
                    int ring = ringValue;
                    if (ring == 1) {
                        Debug.Log("Ring Ring!");
                    }
                }
            }
            catch (System.Exception){
                Debug.LogWarning("Connection with Arduino could not be established!");
            }
        }
    }
}