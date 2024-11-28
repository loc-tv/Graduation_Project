package com.example.pccc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    Button btnphong1, btnphong2;
    int p1 ,p2;
    TextView warning,warning2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnphong1 = findViewById(R.id.btn_p1);
        btnphong2 = findViewById(R.id.btn_p2);
        warning = findViewById(R.id.tv_Warning);
        warning2 = findViewById(R.id.tv_Warning2);
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference datawarning = database.getReference();
        DatabaseReference datawarning1 = database.getReference();
        DatabaseReference datawarning2 = database.getReference();
        datawarning.child("Waring").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try
                {
                    int value = Integer.parseInt(snapshot.getValue().toString());
                    if (value == 0)
                    {
                        warning.setText("CÁC PHÒNG AN TOÀN");
                        warning.setTextColor(Color.GREEN);
                        warning.setTextSize(25);
                    }
                    if (value == 1)
                    {
                        warning.setText("PHÒNG 1 AN TOÀN");
                        warning.setTextColor(Color.GREEN);
                        warning.setTextSize(30);
                    }
                    if (value == 2)
                    {
                        warning.setText("PHÒNG 2 AN TOÀN");
                        warning.setTextColor(Color.GREEN);
                        warning.setTextSize(30);
                    }
                    if (value == 3)
                    {
                        warning.setText("NGUY HIỂM !!!");
                        warning.setTextColor(Color.YELLOW);
                        warning.setTextSize(30);
                    }
                }
                catch (Exception e1)
                {
                }

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });

        datawarning1.child("Room1/RingAlarm").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try
                {
                    p1 = Integer.parseInt(snapshot.getValue().toString());
                    if (p1 == 1 && p2 == 0)
                    {
                        warning2.setText("Phòng 1 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    } else if (p1==0 && p2==1)
                    {
                        warning2.setText("Phòng 2 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    } else if (p1+p2==2)
                    {
                        warning2.setText("Phòng 1,2 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    }else if (p1==0 && p2==0) {
                        warning2.setText(" ");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    }
                }
                catch (Exception e1)
                {
                }

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });

        datawarning2.child("Room2/RingAlarm").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try
                {
                    p2 = Integer.parseInt(snapshot.getValue().toString());
                    if (p1 == 1 && p2 == 0)
                    {
                        warning2.setText("Phòng 1 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    } else if (p1==0 && p2==1)
                    {
                        warning2.setText("Phòng 2 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    } else if (p1+p2==2)
                    {
                        warning2.setText("Phòng 1,2 có người");
                        warning2.setTextColor(Color.YELLOW);
                        warning2.setTextSize(30);
                    }
                }
                catch (Exception e1)
                {
                }

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });





        btnphong1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent myintent = new Intent(MainActivity.this,phong1.class);
                startActivity(myintent);
            }
        });
        btnphong2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent myintent = new Intent(MainActivity.this,phong2.class);
                startActivity(myintent);
            }
        });


       /* if (p1 == 1 && p2 == 0)
        {
            warning2.setText("Phòng 1 có người");
            warning2.setTextColor(Color.YELLOW);
            warning2.setTextSize(30);
        } else if (p1==0 && p2==1)
        {
            warning2.setText("Phòng 2 có người");
            warning2.setTextColor(Color.YELLOW);
            warning2.setTextSize(30);
        } else if (p1+p2==2)
        {
            warning2.setText("Phòng 1,2 có người");
            warning2.setTextColor(Color.YELLOW);
            warning2.setTextSize(30);
        }*/
    }
}