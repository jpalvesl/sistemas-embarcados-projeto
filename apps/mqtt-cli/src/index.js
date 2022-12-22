import { info } from 'console';
import mqtt from 'mqtt'

import readline from 'readline'

const client = mqtt.connect('mqtt://mqtt.eclipseprojects.io')
const urlInfo = "/embarcados/info"
const urlAlarme = "/embarcados/alarme_acionado"
var red = '\u001b[31m';
var reset = '\u001b[0m';

client.on("connect", () => {
    console.info("Conectado com sucesso");
    client.subscribe([urlInfo],()=>{})
    client.subscribe([urlAlarme], () =>{})
})

client.on("message", (url,payload) => {
    if (url === urlInfo){
        let lampada
        let alarme
        const informacao = JSON.parse(payload.toString())
        informacao.is_luz_ligada? lampada = "Ligada" : lampada = "Desligada"
        informacao.is_alarme_ligado? alarme= "Ligado" : alarme = "Desligado"

        console.log("\n--------------------")
        
        console.log(`\nTemperatura: ${informacao.temperatura}\nLampada: ${lampada}\nAlarme: ${alarme}\nUmidade: ${informacao.umidade}`)

        console.log("\n--------------------")

    }
    else if (url === urlAlarme){
        console.log(red + ` 
        ░█▀▀█ ▒█░░░ ░█▀▀█ ▒█▀▀█ ▒█▀▄▀█ ▒█▀▀▀ 　 ░█▀▀█ ▀▀█▀▀ ▀█▀ ▒█░░▒█ ░█▀▀█ ▒█▀▀▄ ▒█▀▀▀█ 
        ▒█▄▄█ ▒█░░░ ▒█▄▄█ ▒█▄▄▀ ▒█▒█▒█ ▒█▀▀▀ 　 ▒█▄▄█ ░▒█░░ ▒█░ ░▒█▒█░ ▒█▄▄█ ▒█░▒█ ▒█░░▒█ 
        ▒█░▒█ ▒█▄▄█ ▒█░▒█ ▒█░▒█ ▒█░░▒█ ▒█▄▄▄ 　 ▒█░▒█ ░▒█░░ ▄█▄ ░░▀▄▀░ ▒█░▒█ ▒█▄▄▀ ▒█▄▄▄█`+ reset)
    }
    
})

const lerLinha = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})



function publica() {
    lerLinha.question("\n\nEscolha uma opção\n 1- Lâmpada\n 2- Alarme\n",comando=>{
        if (comando === "1"){
        lerLinha.question("\n O que você deseja fazer?\n 0- Desligar luz\n 1- Ligar luz\n\n",valor => {
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