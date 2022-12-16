import Paho from 'paho-mqtt'
import { useEffect, useState } from 'react';
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native';

const client = new Paho.Client(
  "broker.hivemq.com",
  8000,
  "jpalvesl"
);

export default function App() {
  const [value, setValue] = useState(0);

  function onMessage(message: any) {
    if (message.destinationName === "mqtt-async-test/value") {
      setValue(Number(message.payloadString))
    }
  }

  function changeValue(client: Paho.Client) {
    const message = new Paho.Message((value+1).toString())
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
        <Text>Oi teste</Text>
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
    height: 150,
    alignItems: "center",
    justifyContent: "center"
  }
});
