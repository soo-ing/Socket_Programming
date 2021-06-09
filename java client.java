package networkpro;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Frame;
import java.awt.TextArea;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;
import java.util.StringTokenizer;

public class client extends Frame implements ActionListener, Runnable{
	private static final long serialVersionUID = 1L;

	private TextArea textarea=new TextArea();
	private TextField textfield=new TextField();
	

	private int port=9291; 
	private String addr="211.198.6.51"; 
	private String name="minu";
	private Socket socket;
	
	public client(){
		add(textarea,BorderLayout.CENTER);
		add(textfield,BorderLayout.SOUTH);
		textfield.addActionListener(this);

		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent arg0) {
				System.exit(0);
			}
		});

		setTitle("도서관 도서 검색 프로그램");
		setSize(500,500);
		setVisible(true);
	}

	public static void main(String[] args) {
		new client().connect();
	}
	
	
	public void connect() {
		// 서버에 접속
		try{

			socket=new Socket(addr,port); 
			textarea.setText("서버 연결\n");

		Thread thread=new Thread(this);
		thread.start();

		}catch(Exception e){

			socket=null;

			textarea.setText("오류 발생\n");
		}
	}
	
	@Override
	public void run() {		// 값을 읽어와서 출력하는 부분(recv())
		String message;
		
		try{
			if(socket==null)
				return;

			BufferedReader reader=new BufferedReader(
					new InputStreamReader(socket.getInputStream()));
		
			while((message=reader.readLine()) !=null){
				
				textarea.append(message+"\n");
								
				if(message.equals("")){
					textarea.append("\n");				
				}				
			}		
		}catch(Exception e){
			textarea.append("연결 해제\n");

			socket=null;
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e) { 		// 값을 보내는 부분(send())
		Component component=(Component)e.getSource();

		if(component instanceof TextField){
			String str=textfield.getText().trim();
			
			if(str.length()==0)
				return;
			
			try{
				// 서버에 연결되지 않은 경우
				if(socket==null)
					return;

				//서버에 데이터 전송
				PrintWriter writer=new PrintWriter(socket.getOutputStream(), true);

				writer.println("["+name+"]"+str);
				
				textarea.append("["+name+"]"+str+"\n");

				textfield.setText("");
				textfield.requestFocus();
			}catch(Exception e2){
				textarea.append("서버 접속 해제");
			}
		}
	}
}
