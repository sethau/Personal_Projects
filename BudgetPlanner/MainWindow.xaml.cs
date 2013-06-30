/* Budget Planner
 * author: Seth Denney
 * date: 6/2013
 * */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using Microsoft.Win32;

namespace BudgetPlanner
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //path for load/exit state file
        private static string FILE_PATH = @"C:\BudgetPlanner\";
        private static string STATE_FILE = FILE_PATH + "session.st";

        private int income1Val;
        private int income2Val;
        private int housingVal;
        private int transportationVal;
        private int foodVal;
        private int savingsVal;
        private int insuranceVal;
        private int otherVal;
        private int totalIncome;
        private int taxes;
        private int availableFunds;
        private bool loaded;

        public MainWindow()
        {
            loaded = false;

            InitializeComponent();

            loaded = true;
            loadState();
        }

        private void calculateIncome()
        {
            totalIncome = income1Val + income2Val;

            calculateTaxes();
        }

        //Based on current federal/state/local income tax approximations,
        //this takes into account the increased tax rates for higher incomes (to a point),
        //but ignores all deductions, loopholes, exceptions, etc.
        //This method can be updated to be more detailed and up-to-date as needed.
        //TL;DR I asked a real accountant! :D
        private void calculateTaxes()
        {
            int tempIncome = totalIncome;
            double tempTaxes = 0;

            if (tempIncome >= 18000)
            {
                tempTaxes += .1 * 18000;
                tempIncome -= 18000;
                if (tempIncome >= 54000)
                {
                    tempTaxes += .15 * 54000;
                    tempIncome -= 54000;
                    tempTaxes += .25 * tempIncome;
                }
                else
                {
                    tempTaxes += .15 * tempIncome;
                }
            }
            else
            {
                tempTaxes += .1 * tempIncome;
            }

            tempTaxes += .0765 * totalIncome;
            tempTaxes += .09 * totalIncome;
            taxes = (int) tempTaxes;
            availableFunds = totalIncome - taxes;

            refreshReports();
        }

        private int calculateRemainingFunds()
        {
            if (monthlyRB.IsChecked == true)
            {
                return (availableFunds / 12) - housingVal - transportationVal - foodVal
                    - (savingsVal / 12) - insuranceVal - otherVal;
            }
            else
            {
                return availableFunds - (housingVal * 12) - (transportationVal * 12) - (foodVal * 12)
                    - savingsVal - (insuranceVal * 12) - (otherVal * 12);
            }
        }

        private void refreshReports()
        {
            if (totalIncome > 0)
            {
                incomeTax.Text = (int)(100 * ((double)taxes / totalIncome)) + "%";
                remaining.Text = calculateRemainingFunds().ToString();
                if (monthlyRB.IsChecked == true)
                {
                    reportTextBox.Text = reportString(true);
                }
                else
                {
                    reportTextBox.Text = reportString(false);
                }
            }
        }

        //saveReport = true will return both yearly and monthly reports side by side through the use of '\t'
        //otherwise, saveReport has a default value of false and is an optional argument
        private string reportString(bool monthly, bool saveReport = false)
        {
            if (saveReport == false)
            {
                if (monthly == true)
                {
                    return "Income 1: $" + income1Val / 12
                            + "\r\nIncome 2: $" + income2Val / 12
                            + "\r\nTotal Income: $" + totalIncome / 12
                            + "\r\nTotal Taxes: $" + taxes / 12
                            + "\r\nSavings " + (int)(savingsSlider.Value * 100) + "%: $" + savingsVal / 12
                            + "\r\n\nHousing: $" + housingVal
                            + "\r\nTransportation: $" + transportationVal
                            + "\r\nFood: $" + foodVal
                            + "\r\nInsurance: $" + insuranceVal
                            + "\r\nOther Expenses: $" + otherVal
                            + "\r\n\nRemaining Balance: $" + remaining.Text;
                }
                else
                {
                    return "Income 1: $" + income1Val
                            + "\r\nIncome 2: $" + income2Val
                            + "\r\nTotal Income: $" + totalIncome
                            + "\r\nTotal Taxes: $" + taxes
                            + "\r\nSavings " + (int)(savingsSlider.Value * 100) + "%: $" + savingsVal
                            + "\r\n\nHousing: $" + housingVal * 12
                            + "\r\nTransportation: $" + transportationVal * 12
                            + "\r\nFood: $" + foodVal * 12
                            + "\r\nInsurance: $" + insuranceVal * 12
                            + "\r\nOther Expenses: $" + otherVal * 12
                            + "\r\n\nRemaining Balance: $" + remaining.Text;
                }
            }
            else
            {
                return "Yearly Budget\t\t\t\tMonthly Budget\r\n\n"
                        + "Income 1: $" + income1Val + "\t\t\tIncome 1: $" + income1Val / 12
                        + "\r\nIncome 2: $" + income2Val + "\t\t\tIncome 2: $" + income2Val / 12
                        + "\r\nTotal Income: $" + totalIncome + "\t\tTotal Income: $" + totalIncome / 12
                        + "\r\nTotal Taxes: $" + taxes + "\t\t\tTotal Taxes: $" + taxes / 12
                        + "\r\nSavings " + (int)(savingsSlider.Value * 100) + "%: $" + savingsVal
                        + "\t\t\tSavings " + (int)(savingsSlider.Value * 100) + "%: $" + savingsVal / 12
                        + "\r\n\nHousing: $" + housingVal * 12 + "\t\t\t\tHousing: $" + housingVal
                        + "\r\nTransportation: $" + transportationVal * 12 + "\t\tTransportation: $" + transportationVal
                        + "\r\nFood: $" + foodVal * 12 + "\t\t\t\t\tFood: $" + foodVal
                        + "\r\nInsurance: $" + insuranceVal * 12 + "\t\t\tInsurance: $" + insuranceVal
                        + "\r\nOther Expenses: $" + otherVal * 12 + "\t\tOther Expenses: $" + otherVal
                        + "\r\n\nRemaining Balance: $" + remaining.Text + "\tRemaining Balance: $" + remaining.Text; ;
            }
        }

        //called upon application exit to save the current state
        private void saveState(object sender, EventArgs e)
        {
            Directory.CreateDirectory(FILE_PATH);
            StreamWriter stateFile = new StreamWriter(STATE_FILE);
            stateFile.WriteLine(income1Min.Text);
            stateFile.WriteLine(income1Slider.Value);
            stateFile.WriteLine(income1Max.Text);
            stateFile.WriteLine(income2Min.Text);
            stateFile.WriteLine(income2Slider.Value);
            stateFile.WriteLine(income2Max.Text);
            stateFile.WriteLine(housingMin.Text);
            stateFile.WriteLine(housingSlider.Value);
            stateFile.WriteLine(housingMax.Text);
            stateFile.WriteLine(transportationMin.Text);
            stateFile.WriteLine(transportationSlider.Value);
            stateFile.WriteLine(transportationMax.Text);
            stateFile.WriteLine(foodMin.Text);
            stateFile.WriteLine(foodSlider.Value);
            stateFile.WriteLine(foodMax.Text);
            stateFile.WriteLine(insuranceMin.Text);
            stateFile.WriteLine(insuranceSlider.Value);
            stateFile.WriteLine(insuranceMax.Text);
            stateFile.WriteLine(savingsSlider.Value);
            stateFile.WriteLine(otherMin.Text);
            stateFile.WriteLine(otherSlider.Value);
            stateFile.WriteLine(otherMax.Text);
            stateFile.Close();
        }

        //called upon application load to load previous state (if exists)
        private void loadState()
        {
            if (File.Exists(STATE_FILE))
            {
                StreamReader stateFile = new StreamReader(STATE_FILE);
                income1Min.Text = stateFile.ReadLine();
                income1Slider.Value = double.Parse(stateFile.ReadLine());
                income1Max.Text = stateFile.ReadLine();
                income2Min.Text = stateFile.ReadLine();
                income2Slider.Value = double.Parse(stateFile.ReadLine());
                income2Max.Text = stateFile.ReadLine();
                housingMin.Text = stateFile.ReadLine();
                housingSlider.Value = double.Parse(stateFile.ReadLine());
                housingMax.Text = stateFile.ReadLine();
                transportationMin.Text = stateFile.ReadLine();
                transportationSlider.Value = double.Parse(stateFile.ReadLine());
                transportationMax.Text = stateFile.ReadLine();
                foodMin.Text = stateFile.ReadLine();
                foodSlider.Value = double.Parse(stateFile.ReadLine());
                foodMax.Text = stateFile.ReadLine();
                insuranceMin.Text = stateFile.ReadLine();
                insuranceSlider.Value = double.Parse(stateFile.ReadLine());
                insuranceMax.Text = stateFile.ReadLine();
                savingsSlider.Value = double.Parse(stateFile.ReadLine());
                otherMin.Text = stateFile.ReadLine();
                otherSlider.Value = double.Parse(stateFile.ReadLine());
                otherMax.Text = stateFile.ReadLine();
                stateFile.Close();
            }
            refreshReports();
        }

        //This method will provide a universally safe
        //method of checking text box int values, and
        //as such is the getter method for all text boxes.
        private int getInt(TextBox txtBox)
        {
            int val;
            if (int.TryParse(txtBox.Text, out val))
            {
                return val;
            }
            else
            {
                return 0;
            }
        }

        private void income1Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                income1Val = (int) (income1Slider.Value * (getInt(income1Max) - getInt(income1Min))
                    + getInt(income1Min));
                calculateIncome();
            }
        }

        private void income1Min_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                income1Val = (int)(income1Slider.Value * (getInt(income1Max) - getInt(income1Min))
                    + getInt(income1Min));
                calculateIncome();
            }
        }

        private void income1Max_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                income1Val = (int)(income1Slider.Value * (getInt(income1Max) - getInt(income1Min))
                    + getInt(income1Min));
                calculateIncome();
            }
        }

        private void income2Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                income2Val = (int)(income2Slider.Value * (getInt(income2Max) - getInt(income2Min))
                    + getInt(income2Min));
                calculateIncome();
            }
        }

        private void income2Min_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                income2Val = (int) (income2Slider.Value * (getInt(income2Max) - getInt(income2Min))
                    + getInt(income2Min));
                calculateIncome();
            }
        }

        private void income2Max_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                income2Val = (int)(income2Slider.Value * (getInt(income2Max) - getInt(income2Min))
                    + getInt(income2Min));
                calculateIncome();
            }
        }

        private void housingSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                housingVal = (int) (housingSlider.Value * (getInt(housingMax) - getInt(housingMin))
                    + getInt(housingMin));
                refreshReports();
            }
        }

        private void housingMin_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                housingVal = (int)(housingSlider.Value * (getInt(housingMax) - getInt(housingMin))
                    + getInt(housingMin));
                refreshReports();
            }
        }

        private void housingMax_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                housingVal = (int)(housingSlider.Value * (getInt(housingMax) - getInt(housingMin))
                    + getInt(housingMin));
                refreshReports();
            }
        }

        private void transportationSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                transportationVal = (int) (transportationSlider.Value * (getInt(transportationMax) - getInt(transportationMin))
                    + getInt(transportationMin));
                refreshReports();
            }
        }

        private void transportationMin_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                transportationVal = (int) (transportationSlider.Value * (getInt(transportationMax) - getInt(transportationMin))
                    + getInt(transportationMin));
                refreshReports();
            }
        }

        private void transportationMax_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                transportationVal = (int) (transportationSlider.Value * (getInt(transportationMax) - getInt(transportationMin))
                    + getInt(transportationMin));
                refreshReports();
            }
        }

        private void foodSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                foodVal = (int) (foodSlider.Value * (getInt(foodMax) - getInt(foodMin))
                    + getInt(foodMin));
                refreshReports();
            }
        }

        private void foodMin_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                foodVal = (int) (foodSlider.Value * (getInt(foodMax) - getInt(foodMin))
                    + getInt(foodMin));
                refreshReports();
            }
        }

        private void foodMax_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                foodVal = (int) (foodSlider.Value * (getInt(foodMax) - getInt(foodMin))
                    + getInt(foodMin));
                refreshReports();
            }
        }

        private void insuranceSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                insuranceVal = (int) (insuranceSlider.Value * (getInt(insuranceMax) - getInt(insuranceMin))
                    + getInt(insuranceMin));
                refreshReports();
            }
        }

        private void insuranceMin_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                insuranceVal = (int) (insuranceSlider.Value * (getInt(insuranceMax) - getInt(insuranceMin))
                    + getInt(insuranceMin));
                refreshReports();
            }
        }

        private void insuranceMax_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                insuranceVal = (int) (insuranceSlider.Value * (getInt(insuranceMax) - getInt(insuranceMin))
                    + getInt(insuranceMin));
                refreshReports();
            }
        }

        private void savingsSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                savingsVal = (int)(savingsSlider.Value * availableFunds);
                refreshReports();
            }
        }

        private void otherSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (loaded)
            {
                otherVal = (int) (otherSlider.Value * (getInt(otherMax) - getInt(otherMin))
                    + getInt(otherMin));
                refreshReports();
            }
        }

        private void otherMin_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                otherVal = (int) (otherSlider.Value * (getInt(otherMax) - getInt(otherMin))
                    + getInt(otherMin));
                refreshReports();
            }
        }

        private void otherMax_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (loaded)
            {
                otherVal = (int) (otherSlider.Value * (getInt(otherMax) - getInt(otherMin))
                    + getInt(otherMin));
                refreshReports();
            }
        }

        private void monthlyRB_Checked(object sender, RoutedEventArgs e)
        {
            refreshReports();
        }

        private void yearlyRB_Checked(object sender, RoutedEventArgs e)
        {
            refreshReports();
        }

        private void saveReportButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.DefaultExt = ".txt";
            dlg.Filter = "Text documents (.txt)|*.txt";
            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                StreamWriter reportFile = new StreamWriter(dlg.FileName);
                reportFile.WriteLine(reportString(true, true));
                reportFile.Close();
            }
        }
    }
}
