/*
 ============================================================================
 Name        : MPIRUN_ADGPRS.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
//using namespace MPI;

int main(int argc, char *argv[]) {
	string adgprs;
	int num_case=1;
	vector<string> case_list_all;
//	int num_procs_tmp;
	if(argc >= 2){
		ifstream in(argv[1]);
		//in >> adgprs >> num_case >> num_procs_tmp;
		in >> adgprs >> num_case;
		string temp;
		for(int i=0;i<num_case;i++){
			in >> temp;
			case_list_all.push_back(temp);
//			temp.clear();
		}
	}else{
		throw runtime_error("Please specify input file...");
	}

	cout << "ADGPRS_CMD: " << adgprs << endl;
	cout << "Number of cases: " << num_case << endl;

	int rank, num_procs;

//	Init(argc, argv);
//	num_procs = COMM_WORLD.Get_size();
//	rank = COMM_WORLD.Get_rank();

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	cout << "rank: "<< rank << endl;
	cout << "Number of processors: " << num_procs << endl;

	//num_procs = num_procs_tmp;

	// Assign cases to processors
	int ave_num_job = num_case / num_procs;
	cout << "Average Number of Jobs:" << ave_num_job << endl;
	int remained_job = num_case % num_procs;
	cout << "Remained Number of Jobs:" << remained_job << endl;
	int ind_job_start, ind_job_end;

	if(rank < remained_job){
		ind_job_start = (ave_num_job + 1) * rank;
		ind_job_end = ind_job_start + ave_num_job;
	}else{
		ind_job_start = remained_job + rank*(ave_num_job);
		ind_job_end = ind_job_start + ave_num_job - 1;
	}

	cout << "Proc #"<< rank <<":Index of first job" << ind_job_start << endl;
	cout << "Proc #"<< rank <<":Index of last job" << ind_job_end << endl;

	vector<string> case_list;
	if(ind_job_start < num_case)
		for(int i=ind_job_start; i <= ind_job_end; i++)
			case_list.push_back(case_list_all[i]);

	// Test case assignment
//	string test_fn;
//	stringstream ss;
//	ss << rank;
//	ss >> test_fn;
//	ofstream out(test_fn.c_str());
//	for(int i=0;i<case_list.size();i++)
//		out << case_list[i] << endl;
//	out.close();

	// Run ADGPRS
	string adgprs_output;
	string rank_str;
	stringstream ss2;
	ss2 << rank;
	ss2 >> rank_str;
	adgprs_output = "adgprs_out_" + rank_str + ".txt";

	for(int i=0;i<case_list.size();i++){
		string run_adgprs_cmd = adgprs + " " + case_list[i] +" 1 0 >> " + adgprs_output;
		cout << run_adgprs_cmd << endl;
		system(run_adgprs_cmd.c_str());
//		out << run_adgprs_cmd << endl;
	}
//	out.close();
	MPI_Finalize();
	return 0;
}

