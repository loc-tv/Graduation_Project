package com.example.pccc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class phong1 extends AppCompatActivity {
    Button back01;
    SeekBar seekBar;
    TextView safetem;
    TextView warning;
    TextView nhietdo01,khoi01;
    ImageView den01,ring01,lua01;
    int i=0,j=0,m=0;
    boolean d,b,q;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_phong1);
        back01 = findViewById(R.id.btn_back01);
        seekBar = findViewById(R.id.seekBar);

        nhietdo01 = findViewById(R.id.txt_nhietdo_ht01);
        khoi01 = findViewById(R.id.txt_khoi_ht01);
        den01 = findViewById(R.id.img_den_01);
        ring01 = findViewById(R.id.img_ring_01);
        lua01 = findViewById(R.id.img_fire01);
        warning = findViewById(R.id.txt_war);
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference nhiet = database.getReference();
        DatabaseReference khoi = database.getReference();
        DatabaseReference datawarning = database.getReference();
        DatabaseReference dataring = database.getReference("Room1/RingAlarm");
        DatabaseReference dataden = database.getReference("Room1/Light");
        DatabaseReference datalua = database.getReference("Room1/Fire");
        nhiet.child("Room1/Temperature").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try {

                    String value = snapshot.getValue().toString();
                    nhietdo01.setText(" " + value);
                }catch (Exception e1){}
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });
        khoi.child("Room1/CO").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try {
                    String value = snapshot.getValue().toString();
                    khoi01.setText(" "+value);
                }catch (Exception e1){}

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
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
                        warning.setTextSize(30);
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
        datalua.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try {
                    int value = Integer.parseInt(snapshot.getValue().toString());
                    if (value== 0 ){
                        lua01.setImageResource(R.drawable.fireoff);
                    }
                    else if (value ==1 ){
                        lua01.setImageResource(R.drawable.fireon);
                    }
                }catch (Exception e1){}

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
        dataden.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try {
                    int value = Integer.parseInt(snapshot.getValue().toString());
                    if (value== 0 ){
                        den01.setImageResource(R.drawable.exitoff);
                    }
                    else if (value ==1 ){
                        den01.setImageResource(R.drawable.exiton);
                    }
                }catch (Exception e1){}

            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });


        dataring.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                try {
                    int value = Integer.parseInt(snapshot.getValue().toString());
                    if (value == 1) {
                        ring01.setImageResource(R.drawable.alarm_on);
                    } else if (value == 0) {
                        ring01.setImageResource(R.drawable.alarm_off);
                    }
                }catch (Exception e1){}

            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
        back01.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    Intent myintent = new Intent(phong1.this,MainActivity.class);
                    startActivity(myintent);
                }
                catch (Exception e1){}

            }
        });
    }
}