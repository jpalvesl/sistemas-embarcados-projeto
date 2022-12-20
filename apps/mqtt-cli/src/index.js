import mqtt from 'mqtt'

import readline from 'readline'

const lerLinha = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

function publica() {
    lerLinha.question('Escreva algo: ', valor => {
        console.log(`Você escreveu ${valor}`);
        client.publish('/embarcados/teste', valor.toString())
        console.info("Publish feito com sucesso");
        publica()
    })

}

const client = mqtt.connect('mqtt://mqtt.eclipseprojects.io')

client.on("connect", () => {
    console.info("Conectado com sucesso");
})

setTimeout(publica, 2000)