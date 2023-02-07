package com.zagusindustries.zaguspay;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.time.LocalDateTime;

public class ZagusPayServer {
    public static void main(String[] args) throws Exception {
        System.out.println("Welcome to pay server console");

        while(true){
            try(ServerSocket serverSocket = new ServerSocket(2007)) {

                Socket socket = serverSocket.accept();
                
                InputStream input = socket.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(input));

                OutputStream output = socket.getOutputStream();
                PrintWriter writer = new PrintWriter(output, true);
    
                String cardIDstr = reader.readLine();    // reads a line of text
                
                System.out.println("Connection from: " + socket.getInetAddress().toString() + " | CARD ID: " + cardIDstr);
                
                if(cardIDstr.contains("386250249")){
                    System.out.println(Utils.getHash(cardIDstr));
                    AddTransaction.addTransaction(0, 2, LocalDateTime.now());
                    writer.print("sssss");
                }
                else if(cardIDstr.contains("9248193194"))
                {
                    System.out.println("ddddd CARD \n");
                    writer.print("yyyyy");
                }


            }catch(IOException ex){
                System.err.println("Il server di ZAGUSpay(trademark registrato) non riesce ad avviarsi");
                ex.printStackTrace();
            }
        }

    }
}