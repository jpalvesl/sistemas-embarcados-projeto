import {Client, Message} from 'paho-mqtt'
import { useEffect, useState } from 'react';
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native';

const client = new Client(
  "broker.hivemq.com",
  8000,
  "jpalvesl"
);

export default function App() {
  const [value, setValue] = useState(0);

  function onMessage(message: Message) {
    const topic = message.destinationName
    const data = message.payloadString

    if (topic === "mqtt-async-test/value") {
      setValue(Number(data))
    }
  }

  function changeValue(client: Client) {
    const message = new Message((value+1).toString())
    message.destinationName = "mqtt-async-test/value"
    client.send(message);
  }

  useEffect(() => {
    client.connect({
      onSuccess: () => {
        console.log("Conectou!");
        client.subscribe("mqtt-async-test/value")
        client.onMessageArrived = onMessage;
      },
      onFailure: () => {
        console.log("Falha ao se conectar! :c");
      }
    })
  }, [])
  
  return (
    <View style={styles.container}>
      <Text>Value is: {value}</Text>
      <TouchableOpacity 
        style={styles.button}
        onPress={() => changeValue(client)}
      >
        <Text style={{ color: "#fff", fontSize: 18 }}>Botão</Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  button: {
    width: 150,
    height: 60,
    borderRadius: 16,
    marginTop: 8,
    backgroundColor: "#000",
    alignItems: "center",
    justifyContent: "center"
  }
});
