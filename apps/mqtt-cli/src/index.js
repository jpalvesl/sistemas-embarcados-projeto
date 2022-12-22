import { info } from 'console';
import mqtt from 'mqtt'

import readline from 'readline'

const client = mqtt.connect('mqtt://mqtt.eclipseprojects.io')
const url = "/embarcados/info"

client.on("connect", () => {
    console.info("Conectado com sucesso");
    client.subscribe([url],()=>{})
})

client.on("message", (urlLuz,payload) => {
    let lampada
    let alarme
    const informacao = JSON.parse(payload.toString())
    informacao.is_luz_ligada? lampada = "Ligada" : lampada = "Desligada"
    informacao.is_alarme_ligado? alarme= "Ligado" : alarme = "Desligado"
    
    console.log(`\nTemperatura: ${informacao.temperatura}\n Lampada: ${lampada}\n Alarme: ${alarme}\n Umidade: ${informacao.umidade}`)
})

const lerLinha = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})



function publica() {
    lerLinha.question("\n\nEscolha uma opção\n 1- Lâmpada\n 2- Alarme\n",comando=>{
        if (comando === "1"){
        lerLinha.question("\n O que você deseja fazer?\n 0- Desligar luz\n 1- Ligar luz\n",valor => {
            client.publish('/embarcados/luz',valor.toString())


            
            setTimeout(()=>{client.publish('/embarcados/dados','1')},1000)
            setTimeout(publica,3000)
        })

    }
    if (comando === "2"){
        lerLinha.question("\n O que você deseja fazer?\n 0- Desligar alarme\n 1- Ligar alarme\n",valor => {
            client.publish('/embarcados/alarme',valor.toString())
            setTimeout(()=>{client.publish('/embarcados/dados','1')},1000)
            setTimeout(publica,4000)
        })

    }
    
})
    

}



setTimeout(publica, 2000)