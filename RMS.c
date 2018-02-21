/*******************************************************************************
*
*  In The Name Of ALLAH
*
*  Project Name   :   Restaurant Management System (RMS)
*  Code Version   :   1.0
*    Written by   :   Mortaza Seydi - Zanjan University, IRAN - Winter 2018
*        GitHub   :   https://github.com/Mortaza-Seydi
*
*  Note1 : This Program is Developed On Linux (Ubuntu 16.04 LTS) With "Gtk+ 3.12" And "Glade 3.18.3".
*          You Should Run And Compile Program With This Versions or upper of Them.
*          If You Want To Run It On Other OS Please See : https://www.gtk.org/download/
*
*  Note2 : Before Running Program For First Time Dir "/files" And "/files/files" Must be Created.
*          if not, You Will Get Core Dumped Error.
*
*  Compile (Linux)  :  gcc -export-dynamic -Wall
*                     `pkg-config --cflags --libs gtk+-3.0`
*                      RMS.c -o RMS.out
*                     `pkg-config --cflags --libs gtk+-3.0`
*
*      Run (Linux)  :  ./RMS.out
*
*  Source Code Available on  :   https://github.com/Mortaza-Seydi
*
*******************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/

typedef struct
{
    gchar DataTime[30];
    gchar details[100];
    int number;
    int TotalPrice;
    gchar chef[20];
    gchar waiter[20];
    gchar table[20];
    gchar status[15];

}   ORDER;

typedef struct
{
    gchar name[20];
    gchar category[20];
    gchar details[30];
    gchar table[20];
    gchar status[15];

}   CHEF_WAITER;

typedef struct
{
    int price;
    gchar category[10];
    gchar name[20];
    gchar status[15];

}   FOOD;

typedef struct
{
    int salary;
    gchar category[20];
    gchar name[20];
    gchar gender[7];

}   PEOPLE;

typedef struct
{
    int capacity;
    gchar name[20];
    gchar status[50];
    gchar start[30];
    gchar end[30];
}   TABLE;

/******************************************************************************/

GtkBuilder *builder;
GtkWindow  *StartWindow;
GtkWindow  *MainWindow;
GtkWidget  *AddFoodWindow;
GtkWidget  *AddPeopleWindow;
GtkWidget  *AddTableWindow;
GtkWidget  *AddChefAbility;
GtkWidget  *ReservationWindow;
GtkWidget  *EditFoodWindow;
GtkWidget  *EditPeopleWindow;
GtkWidget  *EditTableWindow;

/******************************************************************************/

void on_button1_clicked ()    // Restaurant Info
{
    GtkEntry *entry1, *entry2;
    gtk_widget_hide (GTK_WIDGET(StartWindow));

    entry1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));
    entry2 = GTK_ENTRY(gtk_builder_get_object(builder, "entry2"));

    const gchar *entry1Text = gtk_entry_get_text (entry1);
    const gchar *entry2Text = gtk_entry_get_text (entry2);

    FILE *RestInfo;
    RestInfo = fopen ("files/RestInfo.txt", "w");
    fprintf(RestInfo, "%s\n%s\n", entry1Text, entry2Text);
    fclose (RestInfo);

    gtk_widget_show (GTK_WIDGET(MainWindow));
    gtk_window_set_title (MainWindow, entry1Text);


}

void on_button2_clicked ()    // Show Add Food Window
{
    gtk_widget_show(AddFoodWindow);
}

void on_button3_clicked ()    // Add New Food
{

    GtkComboBoxText  *categorycombo, *statuscombo;
    GtkLabel         *categorylabel, *statuslabel;
    categorycombo =  GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext1"));
    statuscombo   =  GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext2"));
    categorylabel =  GTK_LABEL (gtk_builder_get_object(builder, "label18"));
    statuslabel   =  GTK_LABEL (gtk_builder_get_object(builder, "label21"));

    GtkEntry  *nameinput = GTK_ENTRY(gtk_builder_get_object(builder, "entry3"));
    GtkLabel  *namelabel =  GTK_LABEL (gtk_builder_get_object(builder, "label19"));

    GtkSpinButton *pricebutton = GTK_SPIN_BUTTON (gtk_builder_get_object(builder, "spinbutton2"));
    GtkLabel  *pricelabel =  GTK_LABEL (gtk_builder_get_object(builder, "label20"));

    gchar *category = NULL;
    category = gtk_combo_box_text_get_active_text (categorycombo);
    gchar *status = NULL;
    status = gtk_combo_box_text_get_active_text (statuscombo);

    const gchar *name = gtk_entry_get_text (nameinput);
    int price = gtk_spin_button_get_value_as_int (pricebutton);

    gboolean categorybool = TRUE, statusbool = TRUE, namebool = TRUE, pricebool = TRUE;

    if (category == NULL)
    {
        gtk_label_set_text (categorylabel, "Error");
        categorybool = FALSE;
    }
    else
        gtk_label_set_text (categorylabel, " ");

    if (status == NULL)
    {
        gtk_label_set_text (statuslabel, "Error");
        statusbool = FALSE;
    }
    else
        gtk_label_set_text (statuslabel, " ");

    if (strcmp(name, "Name") == 0)
    {
        gtk_label_set_text (namelabel, "Error");
        namebool = FALSE;
    }
    else
        gtk_label_set_text (namelabel, " ");

    if (price == 0)
    {
        gtk_label_set_text (pricelabel, "Error");
        pricebool = FALSE;
    }
    else
        gtk_label_set_text (pricelabel, " ");

    if (categorybool && statusbool && namebool && pricebool)
    {
        gtk_widget_hide (AddFoodWindow);

        FOOD food;
        strcpy(food.category, category);
        g_free(category);
        strcpy(food.status, status);
        g_free(status);

        strcpy(food.name, name);
        food.price = price;

        FILE *fp = fopen ("files/Foods.dat","ab");
        fwrite (&food, sizeof(FOOD), 1, fp);
        fclose (fp);

        FILE *fp2 = fopen ("files/NumberOfFoods.txt","r");
        int n=0;
        if (fp2 != NULL)
        {
            fscanf (fp2, "%d", &n);
            fclose (fp2);
        }


        FILE *fp3 = fopen ("files/NumberOfFoods.txt", "w");
        fprintf (fp3, "%d", n+1);
        fclose (fp3);

        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter (textview, &iter);
        gchar text[200];
        sprintf(text, "\nThis Food Aded SucessFully\ncategory : %s\nName : %s\nStatus : %s\nPrice : %d\n",
                food.category, food.name, food.status, food.price);
        gtk_text_buffer_insert (textview, &iter, text, -1);

        gtk_combo_box_set_active (GTK_COMBO_BOX(categorycombo), -1);
        gtk_combo_box_set_active (GTK_COMBO_BOX(statuscombo), -1);
        gtk_label_set_text (categorylabel, " ");
        gtk_label_set_text (statuslabel, " ");
        gtk_label_set_text (namelabel, " ");
        gtk_label_set_text (pricelabel, " ");
        gtk_entry_set_text (nameinput,  "Name");
    }

}

void on_button4_clicked ()    // Show Add People Window
{
    gtk_widget_show(AddPeopleWindow);
}

void on_button5_clicked ()    // Show Add Table Window
{
    gtk_widget_show(AddTableWindow);
}

void on_button6_clicked ()    // Add New People
{

    GtkComboBoxText  *categorycombo, *gendercombo;
    GtkLabel         *categorylabel;
    categorycombo =  GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext3"));
    gendercombo   =  GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext4"));
    categorylabel =  GTK_LABEL (gtk_builder_get_object(builder, "label22"));

    GtkEntry  *nameinput;
    GtkLabel  *namelabel, *salarylabel;
    nameinput   = GTK_ENTRY(gtk_builder_get_object(builder, "entry11"));
    GtkSpinButton *salarybutton = GTK_SPIN_BUTTON (gtk_builder_get_object(builder, "spinbutton1"));
    namelabel   = GTK_LABEL (gtk_builder_get_object(builder, "label24"));
    salarylabel = GTK_LABEL (gtk_builder_get_object(builder, "label23"));

    gchar *category = NULL;
    category = gtk_combo_box_text_get_active_text (categorycombo);
    gchar  *gender = gtk_combo_box_text_get_active_text (gendercombo);

    const gchar *name  = gtk_entry_get_text (nameinput);
    int salary = gtk_spin_button_get_value_as_int (salarybutton);

    gboolean categorybool = TRUE, namebool = TRUE, salarybool = TRUE;

    if (category == NULL)
    {
        gtk_label_set_text (categorylabel, "Error");
        categorybool = FALSE;
    }
    else
        gtk_label_set_text (categorylabel, " ");

    if (strcmp(name, "Name") == 0)
    {
        gtk_label_set_text (namelabel, "Error");
        namebool = FALSE;
    }
    else
        gtk_label_set_text (namelabel, " ");

    if (salary == 0)
    {
        gtk_label_set_text (salarylabel, "Error");
        salarybool = FALSE;
    }
    else
        gtk_label_set_text (salarylabel, " ");

    if (categorybool  && namebool && salarybool)
    {

        if (strcmp(category, "Chef") == 0)
        {
            gtk_widget_show(AddChefAbility);
        }

        gtk_widget_hide (AddPeopleWindow);

        PEOPLE people;
        strcpy(people.category, category);
        g_free(category);
        strcpy(people.gender, gender);
        g_free(gender);

        strcpy(people.name, name);
        people.salary = salary;

        FILE *fp = fopen ("files/Peoples.dat","ab");
        fwrite (&people, sizeof(PEOPLE), 1, fp);
        fclose (fp);

        FILE *fp2 = fopen ("files/NumberOfPeoples.txt","r");
        int n=0;
        if (fp2 != NULL)
        {
            fscanf (fp2, "%d", &n);
            fclose (fp2);
        }

        FILE *fp3 = fopen ("files/NumberOfPeoples.txt", "w");
        fprintf (fp3, "%d", n+1);
        fclose (fp3);

        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter (textview, &iter);
        gchar text[200];
        sprintf(text, "\nThis People Aded SucessFully\ncategory : %s\nName : %s\nGender : %s\nSlary : %d\n",
                people.category, people.name, people.gender, people.salary);
        gtk_text_buffer_insert (textview, &iter, text, -1);

        if (strcmp(people.category, "Chef") == 0)
        {
            GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label29"));
            gtk_label_set_text (label, people.name);
        }

        gtk_combo_box_set_active (GTK_COMBO_BOX(categorycombo), -1);
        gtk_label_set_text (categorylabel, " ");
        gtk_label_set_text (namelabel, " ");
        gtk_label_set_text (salarylabel, " ");
        gtk_entry_set_text (nameinput,  "Name");
    }

}

void on_button7_clicked ()    // Add New Table
{

    GtkSpinButton   *capacitybutton;
    GtkEntry        *nameinput;
    GtkLabel        *capacitylabel, *namelabel;
    capacitybutton = GTK_SPIN_BUTTON (gtk_builder_get_object(builder, "spinbutton3"));
    nameinput      = GTK_ENTRY (gtk_builder_get_object(builder, "entry14"));
    capacitylabel  = GTK_LABEL (gtk_builder_get_object(builder, "label25"));
    namelabel      = GTK_LABEL (gtk_builder_get_object(builder, "label26"));

    int capacity = gtk_spin_button_get_value_as_int (capacitybutton);
    const gchar *name  = gtk_entry_get_text (nameinput);

    gboolean capacitybool = TRUE, namebool = TRUE;

    if (capacity == 0)
    {
        gtk_label_set_text (capacitylabel, "Error");
        capacitybool = FALSE;
    }
    else
        gtk_label_set_text (capacitylabel, " ");

    if (strcmp(name, "Name") == 0)
    {
        gtk_label_set_text (namelabel, "Error");
        namebool = FALSE;
    }
    else
        gtk_label_set_text (namelabel, " ");

    if (capacitybool && namebool)
    {
        gtk_widget_hide (AddTableWindow);

        TABLE table;
        table.capacity = capacity;
        strcpy(table.name, name);
        strcpy(table.status, "\0");
        strcpy(table.start, "\0");
        strcpy(table.end, "\0");

        FILE *fp = fopen ("files/Tabels.txt", "a");
        fwrite (&table, sizeof(TABLE), 1, fp);
        fclose (fp);

        FILE *fp2 = fopen ("files/NumberOfTabels.txt","r");
        int n=0;
        if (fp2 != NULL)
        {
            fscanf (fp2, "%d", &n);
            fclose (fp2);
        }

        FILE *fp3 = fopen ("files/NumberOfTabels.txt", "w");
        fprintf(fp3, "%d", n+1);
        fclose (fp3);

        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter (textview, &iter);
        gchar text[200];
        sprintf(text, "\nThis Table Aded SucessFully\nName : %s\nCapacity : %d\n",
                table.name, table.capacity);
        gtk_text_buffer_insert (textview, &iter, text, -1);

        gtk_label_set_text (capacitylabel, " ");
        gtk_label_set_text (namelabel, " ");
        gtk_entry_set_text (nameinput,  "Name");
    }

}

void on_button8_clicked ()    // Add Chef Ability
{
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore1"));
      GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label29"));
      GtkTreeIter iter;
      gboolean value;
      gchar file[30];
      int counter=0;

      FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
      int n;
      fscanf (fp, "%d", &n);
      fclose (fp);

      gchar *b[n];
      gchar a[n][3];
      int i;
      for (i=0; i<n; i++)
          sprintf(a[i], "%d", i);

      for (i=0; i<n; i++)
          if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
          {
              gtk_tree_model_get (model, &iter, 1, &value, -1);
              if (value)
              {
                  gtk_tree_model_get (model, &iter, 0, &b[i], -1);
                  GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
                  GtkTextIter iter;
                  gtk_text_buffer_get_end_iter (textview, &iter);
                  gchar text[200];
                  sprintf(text, "\nThis Chef Can Cook : %s\n", b[i]);
                  gtk_text_buffer_insert (textview, &iter, text, -1);
                  counter++;
              }
              else
                  b[i] = "\0";
          }

      if(counter == 0)
      {
            GtkLabel *label2 = GTK_LABEL(gtk_builder_get_object(builder, "label28"));
            gtk_label_set_text (label2, "Error");
      }

      else
      {
        int z=0;
        sprintf(file, "%s%s%s%s", "files/", "files/", gtk_label_get_text (label), ".txt");
        FILE *fp = fopen (file, "ab");
          for (i=0; i<n; i++)
              if (strcmp(b[i], "\0") != 0)
              {
                fprintf (fp, "%s\n", b[i]);
                z++;
              }

          sprintf(file, "%s%s%s%s%s", "files/", "files/", "NumberOf", gtk_label_get_text (label), ".txt");
          FILE *fp2 = fopen (file, "wb");
          fprintf(fp2, "%d\n", z);
          fclose(fp2);
          fclose(fp);
          gtk_widget_hide (AddChefAbility);
      }

}

void on_button9_clicked ()    // Show Reservation Window
{
    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    if (fp == NULL)
    {
        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter (textview, &iter);
        gchar text[200];
        sprintf(text, "\nPlease Add Food First\n");
        gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (ReservationWindow);
    }
    else
    {
      gtk_widget_show (ReservationWindow);
      fclose(fp);
    }
    FILE *fp3 = fopen ("files/NumberOfTabels.txt", "r");
    if (fp3 == NULL)
    {
        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter (textview, &iter);
        gchar text[200];
        sprintf(text, "\nPlease Add Table First\n");
        gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (ReservationWindow);
    }
    else
    {
      gtk_widget_show (ReservationWindow);
      fclose(fp3);
    }
    FILE *fp2 = fopen ("files/NumberOfPeoples.txt", "r");
    if (fp2 == NULL)
    {
      GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter (textview, &iter);
      gchar text[200];
      sprintf(text, "\nPlease Add People First\n");
      gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (ReservationWindow);
    }
    else
    {
      gtk_widget_show (ReservationWindow);
      fclose(fp2);
    }

}

int  on_button10_clicked ()   // Add New Reserve
{

    GDateTime *datetime = g_date_time_new_now_local ();
    char *Time;
    Time = g_date_time_format (datetime, "%F -- %T");

    gboolean tablebool = TRUE, waiterbool = TRUE, selectionbool = TRUE;

    GtkComboBoxText *tablecombo = GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext5"));
    GtkLabel        *tablelabel = GTK_LABEL (gtk_builder_get_object(builder, "label39"));
    gchar           *table      = gtk_combo_box_text_get_active_text (tablecombo);
    if (table == NULL)
    {
        gtk_label_set_text (tablelabel, "Error");
        tablebool = FALSE;
    }
    else
        gtk_label_set_text (tablelabel, " ");

    GtkComboBoxText *waitercombo = GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext6"));
    GtkLabel        *waiterlabel = GTK_LABEL (gtk_builder_get_object(builder, "label40"));
    gchar           *waiter      = gtk_combo_box_text_get_active_text (waitercombo);
    if (waiter == NULL)
    {
        gtk_label_set_text (waiterlabel, "Error");
        waiterbool = FALSE;
    }
    else
        gtk_label_set_text (waiterlabel, " ");

    GtkLabel        *Errorlabel = GTK_LABEL (gtk_builder_get_object(builder, "label41"));
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore2"));
    GtkTreeIter iter;
    gboolean value;

    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    int n;
    fscanf (fp, "%d", &n);
    fclose (fp);

    int z[n],c[n],counter=0;
    gchar *b[n][3];
    gchar text[n][100];

    gchar a[n][2];
    int i;
    for (i=0; i<n; i++)
        sprintf(a[i], "%d", i);

    for (i=0; i<n; i++)
        if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
        {
            gtk_tree_model_get (model, &iter, 0, &value, -1);
            if (value)
            {
                gtk_tree_model_get (model, &iter, 1, &b[i][0], -1);
                gtk_tree_model_get (model, &iter, 2, &b[i][1], -1);
                gtk_tree_model_get (model, &iter, 3, &z[i], -1);
                sprintf(text[i], "%s - %s - %d", b[i][0], b[i][1], z[i]);
                gtk_tree_model_get (model, &iter, 4, &c[i], -1);
                if (c[i] == 0)
                {
                    gtk_label_set_text (Errorlabel, "Error in Selecting Foods");
                    selectionbool = FALSE;
                }
                else
                    gtk_label_set_text (Errorlabel, " ");
                gtk_tree_model_get (model, &iter, 5, &b[i][2], -1);
                if (b[i][2] == NULL)
                {
                    gtk_label_set_text (Errorlabel, "Error in Selecting Foods");
                    selectionbool = FALSE;
                }
                else
                    gtk_label_set_text (Errorlabel, " ");
                counter++;
            }
        }

    if (counter == 0)
    {
          gtk_label_set_text (Errorlabel, "Error in Selecting Foods");
          selectionbool = FALSE;
    }

    if (tablebool && waiterbool && selectionbool)
    {
        ORDER ChildOrder[counter];
        CHEF_WAITER onlinechef[counter];

        int w=0;
        for (i=0; i<n; i++)
          if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
          {
              gtk_tree_model_get (model, &iter, 0, &value, -1);
              if (value)
              {
                  strcpy(ChildOrder[w].DataTime, Time);
                  strcpy(ChildOrder[w].details, text[i]);
                  ChildOrder[w].number = c[i];
                  ChildOrder[w].TotalPrice = c[i]*z[i];
                  {
                      gchar t[200];
                      sprintf(t, "%s%s%s", "files/files/NumberOf", b[i][2], ".txt");
                      FILE *zp = fopen (t, "r");
                      int o=0;
                      fscanf(zp, "%d", &o);
                      fclose(zp);
                      sprintf(t, "%s%s%s", "files/files/", b[i][2], ".txt");
                      FILE *zp2 = fopen (t, "r");
                      int y;
                      gchar q[o][100];
                      for (y=0; y<o; y++)
                      {
                          fscanf(zp2, "%s\n", q[y]);
                      }
                      int dd=0;
                      for (y=0; y<o; y++)
                      {
                          if (strcmp(q[y], b[i][1]) == 0)
                          {
                              dd=1;
                          }
                          if (dd == 0)
                          {
                              gtk_label_set_text (Errorlabel, "Error in Selecting Foods");
                              g_free (Time);
                              g_free (table);
                              g_free (waiter);
                              return 1;
                          }
                      }
                      fclose(zp2);
                  }
                  strcpy(ChildOrder[w].chef, b[i][2]);
                  strcpy(ChildOrder[w].waiter, waiter);
                  strcpy(ChildOrder[w].table, table);
                  strcpy(ChildOrder[w].status, "InProgress");

                  strcpy(onlinechef[w].category, "Chef");
                  strcpy(onlinechef[w].name, ChildOrder[w].chef);
                  strcpy(onlinechef[w].details, Time);
                  strcpy(onlinechef[w].table, table);
                  strcpy(onlinechef[w].status, "InProgress");

                  w++;
              }

          }

        ORDER ParentOrder;
        strcpy(ParentOrder.DataTime, Time);
        strcpy(ParentOrder.details, "Click To See");
        ParentOrder.number = counter;
        ParentOrder.TotalPrice = 0;
        for(i=0; i<counter; i++)
            ParentOrder.TotalPrice = ChildOrder[i].TotalPrice + ParentOrder.TotalPrice ;
        strcpy(ParentOrder.chef, "Click To See");
        strcpy(ParentOrder.waiter, waiter);
        strcpy(ParentOrder.table, table);
        strcpy(ParentOrder.status, "InProgress");

        GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
        GtkTextIter iter2;
        gtk_text_buffer_get_end_iter (textview, &iter2);
        gchar textprint[20000];
        sprintf(textprint, "\nThe Order Added SucessFully\nDate Time : %s\nDetails : %s\nNumber : %d\nTotal Price : %d\nchef : %s\nWaiter : %s\nTable : %s\nStatus : %s\n",
                ParentOrder.DataTime, ParentOrder.details, ParentOrder.number, ParentOrder.TotalPrice,
                ParentOrder.chef, ParentOrder.waiter, ParentOrder.table, ParentOrder.status);
        gtk_text_buffer_insert (textview, &iter2, textprint, -1);


        GtkTreeStore *store = GTK_TREE_STORE (gtk_builder_get_object(builder, "treestore1"));
        GtkTreeIter ParentIter, ChildIter;

        gtk_tree_store_append (store, &ParentIter, NULL);
        gtk_tree_store_set    (store, &ParentIter, 0, FALSE, 1, ParentOrder.DataTime,
                                2, ParentOrder.details, 3, ParentOrder.number, 4, ParentOrder.TotalPrice,
                                5, ParentOrder.chef, 6, ParentOrder.waiter, 7, ParentOrder.table,
                                8, ParentOrder.status, -1);
        for(i=0; i<counter; i++)
        {
            gtk_tree_store_append (store, &ChildIter, &ParentIter);
            gtk_tree_store_set    (store, &ChildIter, 0, FALSE, 1, ChildOrder[i].DataTime,
                                    2, ChildOrder[i].details, 3, ChildOrder[i].number, 4, ChildOrder[i].TotalPrice,
                                    5, ChildOrder[i].chef, 6, ChildOrder[i].waiter, 7, ChildOrder[i].table,
                                    8, ChildOrder[i].status, -1);
        }

        FILE *fp = fopen ("files/Orders.dat", "a");
        fwrite (&ParentOrder, sizeof(ORDER), 1, fp);
        fclose (fp);

        FILE *fp2 = fopen ("files/NumberOfOrders.txt","r");
        int n=0;
        if (fp2 != NULL)
        {
            fscanf (fp2, "%d", &n);
            fclose (fp2);
        }

        FILE *fp3 = fopen ("files/NumberOfOrders.txt", "w");
        fprintf(fp3, "%d", n+1);
        fclose (fp3);

        FILE *gp = fopen ("files/Orders.dat", "a");
        fwrite (&ChildOrder, sizeof(ORDER), counter, gp);
        fclose (gp);

        FILE *gp2 = fopen ("files/NumberOfOrders.txt","r");
        fscanf (gp2, "%d", &n);
        fclose (gp2);

        FILE *gp3 = fopen ("files/NumberOfOrders.txt", "w");
        fprintf(gp3, "%d", n+counter);
        fclose (gp3);

        CHEF_WAITER onlinewaiter;
        strcpy(onlinewaiter.category, "Waiter");
        strcpy(onlinewaiter.name, waiter);
        strcpy(onlinewaiter.details, Time);
        strcpy(onlinewaiter.table, table);
        strcpy(onlinewaiter.status, "InProgress");


        FILE *yp = fopen ("files/CHEF_WAITER.dat", "a");
        fwrite (&onlinewaiter, sizeof(CHEF_WAITER), 1, yp);
        fclose (yp);

        FILE *yp2 = fopen ("files/NumberOfCHEF_WAITER.txt","r");
        n=0;
        if (yp2 != NULL)
        {
            fscanf (fp2, "%d", &n);
            fclose (fp2);
        }

        FILE *yp3 = fopen ("files/NumberOfCHEF_WAITER.txt", "w");
        fprintf(yp3, "%d", n+1);
        fclose (yp3);

        FILE *zp = fopen ("files/CHEF_WAITER.dat", "a");
        fwrite (&onlinechef, sizeof(ORDER), counter, zp);
        fclose (zp);

        FILE *zp2 = fopen ("files/NumberOfCHEF_WAITER.txt","r");
        fscanf (zp2, "%d", &n);
        fclose (zp2);

        FILE *zp3 = fopen ("files/NumberOfCHEF_WAITER.txt", "w");
        fprintf(zp3, "%d", n+1);
        fclose (zp3);

        GtkListStore *store5 = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore12"));
        GtkTreeIter iter;

        gtk_list_store_append (store5, &iter);
        gtk_list_store_set    (store5, &iter, 4, onlinewaiter.category, 0, onlinewaiter.name,1, onlinewaiter.details,
                              3, onlinewaiter.table, 2, onlinewaiter.status,-1);
        for(i=0; i<counter; i++)
        {
          gtk_list_store_append (store5, &iter);
          gtk_list_store_set    (store5, &iter, 4, onlinechef[i].category, 0, onlinechef[i].name,1, onlinechef[i].details,
                                3, onlinechef[i].table, 2, onlinechef[i].status, -1);
        }


        gtk_widget_hide (ReservationWindow);
        g_free (Time);
        g_free (table);
        g_free (waiter);
    }

}

void on_button11_clicked ()   // Edit Tables
{
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));
      GtkTreeIter iter;
      gboolean value;

      FILE *fp = fopen ("files/NumberOfTabels.txt", "r");
      int n=0;
      fscanf (fp, "%d", &n);
      fclose (fp);

      FILE *fp2 = fopen ("files/Tabels.txt", "rb");
      TABLE table[n];
      fread (table, sizeof(TABLE), n, fp2);
      fclose(fp2);

      gchar *b;
      int z;
      int i;
      gchar a[n][2];

      for (i=0; i<n; i++)
          sprintf(a[i], "%d", i);

      for (i=0; i<n; i++)
          if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
          {
              gtk_tree_model_get (model, &iter, 0, &value, -1);
              if (value)
              {
                  gtk_tree_model_get (model, &iter, 1, &b, -1);
                  strcpy(table[i].name, b);
                  gtk_tree_model_get (model, &iter, 2, &z, -1);
                  table[i].capacity = z;
                  gtk_tree_model_get (model, &iter, 3, &b, -1);
                  strcpy(table[i].status, b);
                  gtk_tree_model_get (model, &iter, 4, &b, -1);
                  strcpy(table[i].start, b);
                  gtk_tree_model_get (model, &iter, 5, &b, -1);
                  strcpy(table[i].end, b);
                  GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
                  GtkTextIter iter;
                  gtk_text_buffer_get_end_iter (textview, &iter);
                  gchar text[200];
                  sprintf(text, "\nThis Table Edited SucessFully\nCapacity : %d\nName : %s\n",
                          table[i].capacity, table[i].name);
                  gtk_text_buffer_insert (textview, &iter, text, -1);
              }
          }

      FILE *fp3 = fopen ("files/Tabels.txt", "wb");
      fwrite (table, sizeof(TABLE), n, fp3);
      fclose(fp3);

      FILE *gp = fopen ("files/NumberOfTabels.txt","r");
      int n1=0;
      fscanf (gp, "%d", &n1);
      fclose (gp);

      TABLE table2[n1];

      FILE *gp2 = fopen ("files/Tabels.txt", "r");
      fread (table2, sizeof(TABLE), n1, gp2);
      fclose (gp2);


      GtkListStore *store2 = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore13"));
      gtk_list_store_clear (store2);
      GtkTreeIter iter2;

      for (i=0; i<n1; i++)
      {
          if (strcmp(table2[i].status, "Reserved Now") == 0 || strcmp(table2[i].status, "Reserved For Another Time") == 0)
          {
              gtk_list_store_append (store2, &iter2);
              gtk_list_store_set    (store2, &iter2, 0, table2[i].name, 1, table2[i].capacity,
                                            2, table2[i].status, 3, table2[i].start, 4, table2[i].end, -1);
          }
      }

      gtk_widget_hide (EditTableWindow);
}

void on_button12_clicked ()   // Edit People
{
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));
      GtkTreeIter iter;
      gboolean value;

      FILE *fp = fopen ("files/NumberOfPeoples.txt", "r");
      int n=0;
      fscanf (fp, "%d", &n);
      fclose (fp);

      FILE *fp2 = fopen ("files/Peoples.dat", "rb");
      PEOPLE people[n];
      fread (people, sizeof(PEOPLE), n, fp2);
      fclose(fp2);

      gchar *b;
      int z;
      int i;
      gchar a[n][2];

      for (i=0; i<n; i++)
          sprintf(a[i], "%d", i);

      for (i=0; i<n; i++)
          if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
          {
              gtk_tree_model_get (model, &iter, 0, &value, -1);
              if (value)
              {
                  gtk_tree_model_get (model, &iter, 1, &b, -1);
                  strcpy(people[i].category, b);
                  gtk_tree_model_get (model, &iter, 2, &b, -1);
                  strcpy(people[i].name, b);
                  gtk_tree_model_get (model, &iter, 3, &z, -1);
                  people[i].salary = z;
                  gtk_tree_model_get (model, &iter, 4, &b, -1);
                  strcpy(people[i].gender, b);
                  GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
                  GtkTextIter iter;
                  gtk_text_buffer_get_end_iter (textview, &iter);
                  gchar text[200];
                  sprintf(text, "\nThis People Edited SucessFully\ncategory : %s\nName : %s\nGender : %s\nSalary : %d\n",
                          people[i].category, people[i].name, people[i].gender, people[i].salary);
                  gtk_text_buffer_insert (textview, &iter, text, -1);
              }
          }

      FILE *fp3 = fopen ("files/Peoples.dat", "wb");
      fwrite (people, sizeof(PEOPLE), n, fp3);
      fclose(fp3);

      gtk_widget_hide (EditPeopleWindow);
}

void on_button13_clicked ()   // Edit Food
{
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));
      GtkTreeIter iter;
      gboolean value;

      FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
      int n;
      fscanf (fp, "%d", &n);
      fclose (fp);

      FILE *fp2 = fopen ("files/Foods.dat", "rb");
      FOOD food[n];
      fread (food, sizeof(FOOD), n, fp2);
      fclose(fp2);

      gchar *b;
      int z;
      int i;
      gchar a[n][2];

      for (i=0; i<n; i++)
          sprintf(a[i], "%d", i);

      for (i=0; i<n; i++)
          if ( gtk_tree_model_get_iter_from_string (model, &iter, a[i]))
          {
              gtk_tree_model_get (model, &iter, 0, &value, -1);
              if (value)
              {
                  gtk_tree_model_get (model, &iter, 1, &b, -1);
                  strcpy(food[i].category, b);
                  gtk_tree_model_get (model, &iter, 2, &b, -1);
                  strcpy(food[i].name, b);
                  gtk_tree_model_get (model, &iter, 3, &z, -1);
                  food[i].price = z;
                  gtk_tree_model_get (model, &iter, 4, &b, -1);
                  strcpy(food[i].status, b);
                  GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
                  GtkTextIter iter;
                  gtk_text_buffer_get_end_iter (textview, &iter);
                  gchar text[200];
                  sprintf(text, "\nThis Food Edited SucessFully\ncategory : %s\nName : %s\nStatus : %s\nPrice : %d\n",
                          food[i].category, food[i].name, food[i].status, food[i].price);
                  gtk_text_buffer_insert (textview, &iter, text, -1);
              }
          }

      FILE *fp3 = fopen ("files/Foods.dat", "wb");
      fwrite (food, sizeof(FOOD), n, fp3);
      fclose(fp3);

      gtk_widget_hide (EditFoodWindow);

}

void on_button14_clicked ()   // Show Edit Food Window
{
    EditFoodWindow = GTK_WIDGET (gtk_builder_get_object (builder, "EditFoodWindow"));
    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    if (fp == NULL)
    {
      GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter (textview, &iter);
      gchar text[200];
      sprintf(text, "\nPlease Add Food First\n");
      gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (EditFoodWindow);
    }
    else
    {
      gtk_widget_show (EditFoodWindow);
      fclose(fp);
    }
}

void on_button15_clicked ()   // Show Edit People Window
{
    EditPeopleWindow = GTK_WIDGET (gtk_builder_get_object (builder, "EditPeopleWindow"));
    FILE *fp = fopen ("files/NumberOfPeoples.txt", "r");
    if (fp == NULL)
    {
      GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter (textview, &iter);
      gchar text[200];
      sprintf(text, "\nPlease Add People First\n");
      gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (EditPeopleWindow);
    }
    else
    {
      fclose(fp);
      gtk_widget_show (EditPeopleWindow);
    }

}

void on_button16_clicked ()   // Show Edit Table Window
{
    EditTableWindow = GTK_WIDGET (gtk_builder_get_object (builder, "EditTableWindow"));
    FILE *fp = fopen ("files/NumberOfTabels.txt", "r");
    if (fp == NULL)
    {
      GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter (textview, &iter);
      gchar text[200];
      sprintf(text, "\nPlease Add Table First\n");
      gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (EditTableWindow);
    }
    else
    {
      gtk_widget_show (EditTableWindow);
      fclose(fp);
    }
}

void on_button17_clicked ()   // Done a Reserve
{
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "treestore1"));
    GtkTreeIter iter, iter2;
    gtk_tree_model_get_iter_first (model, &iter);
    gboolean value;

    FILE *fp = fopen ("files/NumberOfOrders.txt", "r");
    int n;
    fscanf (fp, "%d", &n);
    fclose (fp);

    FILE *fp2 = fopen ("files/Orders.dat", "rb");
    ORDER order[n];
    fread (order, sizeof(ORDER), n, fp2);
    fclose(fp2);

    int w=0;
    do
    {
            gtk_tree_model_get (model, &iter, 0, &value, -1);
            if (value)
            {
                strcpy(order[w].status, "Done");
                GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
                GtkTextIter iter3;
                gtk_text_buffer_get_end_iter (textview, &iter3);
                gchar text[200];
                sprintf(text, "\nThis Order Doned SucessFully\n");
                gtk_text_buffer_insert (textview, &iter3, text, -1);
                w++;
                if (gtk_tree_model_iter_children (model, &iter2, &iter))
                {
                    do
                    {
                        strcpy(order[w].status, "Done");
                        w++;

                    } while (gtk_tree_model_iter_next (model, &iter2));
                }
            }
            else
                w = w + gtk_tree_model_iter_n_children (model, &iter) + 1;

    } while (gtk_tree_model_iter_next (model, &iter));

    FILE *gp2 = fopen ("files/Orders.dat", "wb");
    fwrite (order, sizeof(ORDER), n, gp2);
    fclose(gp2);

    FILE *fp11 = fopen ("files/NumberOfOrders.txt", "r");
    fscanf (fp11, "%d", &n);
    fclose (fp11);

    FILE *fp6 = fopen ("files/Orders.dat", "rb");
    ORDER order2[n];
    fread (&order2, sizeof(ORDER), n, fp6);
    fclose (fp6);

    GtkTreeStore *store = GTK_TREE_STORE (gtk_builder_get_object(builder, "treestore1"));
    gtk_tree_store_clear (store);
    GtkTreeIter ParentIter, ChildIter;
    int i;
    for (i=0; i<n; i++)
    {
          if (strcmp(order2[i].details, "Click To See") == 0)
          {
              gtk_tree_store_append (store, &ParentIter, NULL);
              gtk_tree_store_set    (store, &ParentIter, 0, FALSE, 1, order2[i].DataTime,
                                      2, order2[i].details, 3, order2[i].number, 4, order2[i].TotalPrice,
                                      5, order2[i].chef, 6, order2[i].waiter, 7, order2[i].table,
                                      8, order2[i].status, -1);
          }
          else
          {
              gtk_tree_store_append (store, &ChildIter, &ParentIter);
              gtk_tree_store_set    (store, &ChildIter, 0, FALSE, 1, order2[i].DataTime,
                                      2, order2[i].details, 3, order2[i].number, 4, order2[i].TotalPrice,
                                      5, order2[i].chef, 6, order2[i].waiter, 7, order2[i].table,
                                      8, order2[i].status, -1);
          }
    }

}

/******************************************************************************/

void on_cellrendererspin1_edited (GtkCellRendererSpin *renderer, gchar *path)
{
      GtkTreeIter iter;
      GtkTreeModel *model;
      GtkAdjustment *adjustment;
      gint value;

      model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore2"));
      adjustment = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
      value = gtk_adjustment_get_value (adjustment);

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 4, value, -1);
}

void on_cellrendererspin2_edited (GtkCellRendererSpin *renderer, gchar *path)
{
      GtkTreeIter iter;
      GtkTreeModel *model;
      GtkAdjustment *adjustment;
      gint value;

      model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));
      adjustment = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
      value = gtk_adjustment_get_value (adjustment);

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 3, value, -1);
}

void on_cellrendererspin4_edited (GtkCellRendererSpin *renderer, gchar *path)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkAdjustment *adjustment;
    gint value;

    model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));
    adjustment = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
    value = gtk_adjustment_get_value (adjustment);

    if (gtk_tree_model_get_iter_from_string (model, &iter, path))
        gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, value, -1);
}

void on_cellrendererspin3_edited (GtkCellRendererSpin *renderer, gchar *path)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkAdjustment *adjustment;
    gint value;

    model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));
    adjustment = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
    value = gtk_adjustment_get_value (adjustment);

    if (gtk_tree_model_get_iter_from_string (model, &iter, path))
        gtk_list_store_set (GTK_LIST_STORE (model), &iter, 3, value, -1);
}

/******************************************************************************/

void on_cellrenderercombo1_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{

    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore2"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 5, new_text, -1);
}

void on_cellrenderercombo2_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 4, new_text, -1);
}

void on_cellrenderercombo3_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
      GtkTreeIter iter;
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));

        if (gtk_tree_model_get_iter_from_string (model, &iter, path))
              gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, new_text, -1);
}

void on_cellrenderercombo4_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
      GtkTreeIter iter;
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));

        if (gtk_tree_model_get_iter_from_string (model, &iter, path))
              gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, new_text, -1);
}

void on_cellrenderercombo5_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 4, new_text, -1);
  }

void on_cellrenderercombo34_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 3, new_text, -1);
}

/******************************************************************************/

void on_cellrenderertext5_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, new_text, -1);
}

void on_cellrenderertext6_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, new_text, -1);
}

void on_cellrenderertext7_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, new_text, -1);
}

void on_cellrenderertext35_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 4, new_text, -1);
}

void on_cellrenderertext36_edited (GtkCellRendererText *renderer, gchar *path, gchar *new_text)
{
    GtkTreeIter iter;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 5, new_text, -1);
}

/******************************************************************************/

void on_cellrenderertoggle1_toggled (GtkCellRendererToggle *renderer, gchar *path)
{

      GtkTreeIter iter;
      gboolean value;
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore1"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
      {
          gtk_tree_model_get (model, &iter, 1, &value, -1);
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, !value, -1);
      }

}

void on_cellrenderertoggle2_toggled (GtkCellRendererToggle *renderer, gchar *path)
{

      GtkTreeIter iter;
      gboolean value;
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore2"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
      {
          gtk_tree_model_get (model, &iter, 0, &value, -1);
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, !value, -1);
      }
}

void on_cellrenderertoggle3_toggled (GtkCellRendererToggle *renderer, gchar *path)
{
      GtkTreeIter iter;
      gboolean value;
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore4"));

      if (gtk_tree_model_get_iter_from_string (model, &iter, path))
      {
          gtk_tree_model_get (model, &iter, 0, &value, -1);
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, !value, -1);
      }
}

void on_cellrenderertoggle4_toggled (GtkCellRendererToggle *renderer, gchar *path)
{
    GtkTreeIter iter;
    gboolean value;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore7"));

    if (gtk_tree_model_get_iter_from_string (model, &iter, path))
    {
        gtk_tree_model_get (model, &iter, 0, &value, -1);
        gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, !value, -1);
    }
}

void on_cellrenderertoggle5_toggled (GtkCellRendererToggle *renderer, gchar *path)
{
    GtkTreeIter iter;
    gboolean value;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore10"));

    if (gtk_tree_model_get_iter_from_string (model, &iter, path))
    {
        gtk_tree_model_get (model, &iter, 0, &value, -1);
        gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, !value, -1);
    }
}

void on_cellrenderertoggle6_toggled (GtkCellRendererToggle *renderer, gchar *path)
{
    GtkTreeIter ParentIter, ChildIter;
    gboolean value;
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "treestore1"));

    if (strlen(path) == 1 || strlen(path) == 2)
        if (gtk_tree_model_get_iter_from_string (model, &ParentIter, path))
        {
            gtk_tree_model_get (GTK_TREE_MODEL(model), &ParentIter, 0, &value, -1);
            gtk_tree_store_set (GTK_TREE_STORE(model), &ParentIter, 0 , !value, -1);
            if (gtk_tree_model_iter_children (model, &ChildIter, &ParentIter))
            do
            {
                gtk_tree_store_set (GTK_TREE_STORE(model), &ChildIter, 0 , !value, -1);

            }   while (gtk_tree_model_iter_next (model, &ChildIter));

        }

    if (strlen(path) == 3 || strlen(path) == 4)
    {
        if (gtk_tree_model_get_iter_from_string (model, &ChildIter, path))
        {
            gtk_tree_model_get (GTK_TREE_MODEL(model), &ChildIter, 0, &value, -1);
            gtk_tree_store_set (GTK_TREE_STORE(model), &ChildIter, 0 , !value, -1);
            gtk_tree_model_iter_parent (model, &ParentIter, &ChildIter);
            gtk_tree_store_set (GTK_TREE_STORE(model), &ParentIter, 0 , FALSE, -1);
        }
    }


}

/******************************************************************************/

void on_MainWindow_show ()
{

    FILE *fp = fopen ("files/NumberOfOrders.txt","r");
    int n=0;
    if (fp != NULL)
    {
      fscanf (fp, "%d", &n);
      fclose (fp);
      ORDER order[n];

      FILE *fp2 = fopen ("files/Orders.dat", "r");
      fread (&order, sizeof(ORDER), n, fp2);
      fclose (fp2);


      GtkTreeStore *store = GTK_TREE_STORE (gtk_builder_get_object(builder, "treestore1"));
      gtk_tree_store_clear (store);
      GtkTreeIter ParentIter, ChildIter;
      int i;
      for (i=0; i<n; i++)
      {
          if (strcmp(order[i].details, "Click To See") == 0)
          {
              gtk_tree_store_append (store, &ParentIter, NULL);
              gtk_tree_store_set    (store, &ParentIter, 0, FALSE, 1, order[i].DataTime,
                                      2, order[i].details, 3, order[i].number, 4, order[i].TotalPrice,
                                      5, order[i].chef, 6, order[i].waiter, 7, order[i].table,
                                      8, order[i].status, -1);
          }
          else
          {
              gtk_tree_store_append (store, &ChildIter, &ParentIter);
              gtk_tree_store_set    (store, &ChildIter, 0, FALSE, 1, order[i].DataTime,
                                      2, order[i].details, 3, order[i].number, 4, order[i].TotalPrice,
                                      5, order[i].chef, 6, order[i].waiter, 7, order[i].table,
                                      8, order[i].status, -1);
          }
      }


      FILE *gp = fopen ("files/NumberOfTabels.txt","r");
      n=0;
      if (fp != NULL)
      {
        fscanf (gp, "%d", &n);
        fclose (gp);
      }

      TABLE table[n];

      FILE *gp2 = fopen ("files/Tabels.txt", "r");
      fread (table, sizeof(TABLE), n, gp2);
      fclose (gp2);


      GtkListStore *store2 = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore13"));
      gtk_list_store_clear (store2);
      GtkTreeIter iter;

      for (i=0; i<n; i++)
      {
          if (strcmp(table[i].status, "Reserved Now") == 0 || strcmp(table[i].status, "Reserved For Another Time") == 0)
          {
              gtk_list_store_append (store2, &iter);
              gtk_list_store_set    (store2, &iter, 0, table[i].name, 1, table[i].capacity,
                                      2, table[i].status, 3, table[i].start, 4, table[i].end, -1);
          }
      }
    }

    GtkListStore *store5 = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore12"));
    GtkTreeIter iter5;

    FILE *zp2 = fopen ("files/NumberOfCHEF_WAITER.txt", "r");
    n=0;
    if (zp2 != NULL)
    {
        fscanf (zp2, "%d", &n);
        fclose (zp2);
        CHEF_WAITER online[n];

        FILE *zp3 = fopen ("files/CHEF_WAITER.dat", "rb");
        fread (online, sizeof(CHEF_WAITER), n, zp3);
        fclose (zp3);

        int i;
        for(i=0; i<n; i++)
        {
          gtk_list_store_append (store5, &iter5);
          gtk_list_store_set    (store5, &iter5, 4, online[i].category, 0, online[i].name,1, online[i].details,
                                  3, online[i].table, 2, online[i].status, -1);
        }
    }


}

void on_ResevationWindow_show ()
{
    GtkListStore *store = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore2"));
    GtkTreeIter   iter, iter2;
    gtk_list_store_clear (store);

    GtkCellRenderer *renderer2 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo1"));
    GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore3"));
    gtk_list_store_clear (GTK_LIST_STORE(model));
    g_object_set (renderer2, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model, NULL);

    GtkCellRenderer *renderer;
    GtkAdjustment *adj;
    adj = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
    renderer = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrendererspin1"));
    g_object_set (renderer, "editable", TRUE, "adjustment", adj, "digits", 0, NULL);

    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    int n=0;
    fscanf (fp, "%d", &n);
    fclose (fp);

    FILE *fp2 = fopen ("files/Foods.dat", "rb");
    FOOD food[n];
    fread (food, sizeof(FOOD), n, fp2);
    fclose(fp2);

    int i;
    for (i=0; i<n; i++)
    {
        if (strcmp(food[i].status, "Available") == 0)
        {
          gtk_list_store_append (store, &iter);
          gtk_list_store_set (store, &iter, 0, FALSE, 1, food[i].category, 2,
              food[i].name, 3, food[i].price, -1);
        }
    }

    FILE *pp = fopen ("files/NumberOfPeoples.txt", "r");
    fscanf (pp, "%d", &n);
    fclose (pp);

    FILE *pp2 = fopen ("files/Peoples.dat", "rb");
    PEOPLE people[n];
    fread (people, sizeof(PEOPLE), n, pp2);
    fclose(pp2);

    GtkComboBoxText *waitercombo = GTK_COMBO_BOX_TEXT (gtk_builder_get_object(builder, "comboboxtext6"));
    gtk_combo_box_text_remove_all (waitercombo);

    for (i=0; i<n; i++)
    {
        if (strcmp(people[i].category, "Chef") == 0)
        {
            gtk_list_store_append (GTK_LIST_STORE(model), &iter2);
            gtk_list_store_set (GTK_LIST_STORE(model), &iter2, 0, people[i].name, -1);
        }
        if (strcmp(people[i].category, "Waiter") == 0 || strcmp(people[i].category, "Waitress") == 0)
            gtk_combo_box_text_insert_text (waitercombo, i, people[i].name);
    }

    GtkComboBoxText *tablecombo = GTK_COMBO_BOX_TEXT (gtk_builder_get_object (builder, "comboboxtext5"));
    gtk_combo_box_text_remove_all (tablecombo);

    FILE *gp = fopen ("files/NumberOfTabels.txt", "r");
    fscanf (gp, "%d", &n);
    fclose (gp);

    FILE *gp2 = fopen ("files/Tabels.txt", "rb");
    TABLE table[n];
    fread (table, sizeof(TABLE), n, gp2);
    fclose(gp2);

    char text[30];

    for (i=0; i<n; i++)
    {
          if (strcmp(table[i].status, "Reserved Now") != 0)
          {
            sprintf (text, "%s%s%d", table[i].name, " -> ", table[i].capacity);
            gtk_combo_box_text_insert_text (tablecombo, i, text);
          }

    }


}

void on_AddPeopleWindow_show ()
{
    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    if (fp != NULL)
        fclose (fp);
    else
    {
      GtkTextBuffer *textview = GTK_TEXT_BUFFER (gtk_builder_get_object(builder, "textbuffer1"));
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter (textview, &iter);
      gchar text[200];
      sprintf(text, "\nPlease Add Food First\n");
      gtk_text_buffer_insert (textview, &iter, text, -1);
        gtk_widget_hide (AddPeopleWindow);
    }
}

void on_AddChefAbility_show ()
{
    GtkListStore *store = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore1"));
    GtkTreeIter   iter;
    gtk_list_store_clear (store);

    FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
    int n=0;
    fscanf (fp, "%d", &n);
    fclose (fp);

    FILE *fp2 = fopen ("files/Foods.dat", "rb");
    FOOD food[n];
    fread (food, sizeof(FOOD), n, fp2);
    fclose(fp2);

    int i;
    for (i=0; i<n; i++)
    {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 1, FALSE, 0, food[i].name, -1);
    }
}

void on_EditTableWindow_show ()
{

    FILE *fp = fopen ("files/NumberOfTabels.txt", "r");
    int n=0;
    if (fp != NULL)
    {
      fscanf (fp, "%d", &n);
      fclose (fp);
      GtkListStore *store = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore10"));
      GtkTreeIter   iter;
      gtk_list_store_clear (store);

      GtkCellRenderer *renderer;
      GtkAdjustment *adj;
      adj = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
      renderer = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrendererspin4"));
      g_object_set (renderer, "editable", TRUE, "adjustment", adj, "digits", 0, NULL);

      GtkCellRenderer *renderer2 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo34"));
      GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore11"));
      g_object_set (renderer2, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model, NULL);

      FILE *fp2 = fopen ("files/Tabels.txt", "rb");
      TABLE table[n];
      fread (table, sizeof(TABLE), n, fp2);
      fclose(fp2);

      int i;
      for (i=0; i<n; i++)
      {
          gtk_list_store_append (store, &iter);
          gtk_list_store_set (store, &iter, 0, FALSE, 1, table[i].name, 2, table[i].capacity,
                              3, table[i].status, 4, table[i].start, 5, table[i].end, -1);

      }
    }

}

void on_EditFoodWindow_show ()
{

      FILE *fp = fopen ("files/NumberOfFoods.txt", "r");
      int n=0;
      if (fp != NULL)
      {
        fscanf (fp, "%d", &n);
        fclose (fp);
        GtkListStore *store = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore4"));
        GtkTreeIter   iter;
        gtk_list_store_clear (store);

        GtkCellRenderer *renderer2 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo2"));
        GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore5"));
        g_object_set (renderer2, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model, NULL);

        GtkCellRenderer *renderer3 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo3"));
        GtkTreeModel *model1 = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore6"));
        g_object_set (renderer3, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model1, NULL);


        GtkCellRenderer *renderer;
        GtkAdjustment *adj;
        adj = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
        renderer = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrendererspin2"));
        g_object_set (renderer, "editable", TRUE, "adjustment", adj, "digits", 0, NULL);

        FILE *fp2 = fopen ("files/Foods.dat", "rb");
        FOOD food[n];
        fread (food, sizeof(FOOD), n, fp2);
        fclose(fp2);

        int i;
        for (i=0; i<n; i++)
        {
            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter, 0, FALSE, 1, food[i].category, 2,
                food[i].name, 3, food[i].price, 4, food[i].status, -1);

        }
      }
}

void on_EdotPeopleWindow_show ()
{

      FILE *fp = fopen ("files/NumberOfPeoples.txt", "r");
      int n=0;
      if (fp != NULL)
      {
        fscanf (fp, "%d", &n);
        fclose (fp);
        GtkListStore *store = GTK_LIST_STORE (gtk_builder_get_object(builder, "liststore7"));
        GtkTreeIter   iter;
        gtk_list_store_clear (store);

        GtkCellRenderer *renderer2 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo4"));
        GtkTreeModel *model = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore8"));
        g_object_set (renderer2, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model, NULL);

        GtkCellRenderer *renderer3 = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrenderercombo5"));
        GtkTreeModel *model2 = GTK_TREE_MODEL (gtk_builder_get_object(builder, "liststore9"));
        g_object_set (renderer3, "text-column", 0, "editable", TRUE, "has-entry", TRUE, "model", model2, NULL);

        GtkCellRenderer *renderer;
        GtkAdjustment *adj;
        adj = GTK_ADJUSTMENT (gtk_builder_get_object(builder, "adjustment1"));
        renderer = GTK_CELL_RENDERER (gtk_builder_get_object(builder, "cellrendererspin3"));
        g_object_set (renderer, "editable", TRUE, "adjustment", adj, "digits", 0, NULL);

        FILE *fp2 = fopen ("files/Peoples.dat", "rb");
        PEOPLE people[n];
        fread (people, sizeof(PEOPLE), n, fp2);
        fclose(fp2);

        int i;
        for (i=0; i<n; i++)
        {
            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter, 0, FALSE, 1, people[i].category, 2,
                people[i].name, 3, people[i].salary, 4, people[i].gender, -1);

        }
      }
}

/******************************************************************************/

void on_imagemenuitem10_activate ()
{
    GtkWidget *about = GTK_WIDGET (gtk_builder_get_object (builder, "aboutdialog1"));
    gtk_widget_show (about);
}

/******************************************************************************/

int main (int argc, char *argv[])
{

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file ("GUI.glade");
    gtk_builder_connect_signals(builder, NULL);

    StartWindow = GTK_WINDOW(gtk_builder_get_object(builder, "StartWindow"));
    MainWindow  = GTK_WINDOW(gtk_builder_get_object(builder, "MainWindow"));
    AddFoodWindow = GTK_WIDGET(gtk_builder_get_object(builder, "AddFoodWindow"));
    AddPeopleWindow = GTK_WIDGET(gtk_builder_get_object(builder, "AddPeopleWindow"));
    AddTableWindow = GTK_WIDGET(gtk_builder_get_object(builder, "AddTableWindow"));
    AddChefAbility = GTK_WIDGET(gtk_builder_get_object(builder, "AddChefAbility"));
    ReservationWindow  = GTK_WIDGET(gtk_builder_get_object(builder, "ResevationWindow"));

    gtk_widget_hide (GTK_WIDGET(AddFoodWindow));
    gtk_widget_hide (GTK_WIDGET(AddPeopleWindow));
    gtk_widget_hide (GTK_WIDGET(AddTableWindow));
    gtk_widget_hide (GTK_WIDGET(MainWindow));
    gtk_widget_hide (GTK_WIDGET(StartWindow));
    gtk_widget_hide (GTK_WIDGET(AddChefAbility));
    gtk_widget_hide (GTK_WIDGET(ReservationWindow));

    GtkLabel *TimeLabel = GTK_LABEL (gtk_builder_get_object(builder, "label32"));
    GDateTime *datetime = g_date_time_new_now_local ();
    char *Time;
    Time = g_date_time_format (datetime, "%F -- %T");
    gtk_label_set_text (TimeLabel, Time);
    g_free(Time);

    FILE *RestInfo;
    RestInfo = fopen ("files/RestInfo.txt","rt");
    if(RestInfo == NULL)
    {
        gtk_widget_show (GTK_WIDGET(StartWindow));
    }

    else
    {
        gtk_widget_show (GTK_WIDGET(MainWindow));
        gchar a[20];
        fscanf(RestInfo, "%s\n", a);
        gtk_window_set_title (MainWindow, a);
        fclose(RestInfo);
    }

    gtk_main();

    return 0;
}
