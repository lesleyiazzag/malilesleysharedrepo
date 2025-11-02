#include "vm_pager.h"
#include <iostream>
//#include <std>                                                                                                                                                                                            
#include <queue>
#include<map>
#include <cstdint>
#include <cstring>
using namespace std;


//data structures                                                                                                                                                                                     
//struct for each virtual page including extra information not kept in page table entry                                                                                                                    
struct vm_page{
  int pte;
  int dirty=0;
  int reference = 0;
  unsigned int disk_location;
  bool zero = true;
};

//addresses are from the starting place in mem, find pte number by dividing by page size                                                                                                            
//struct for each process, keeps process id, hardware page table, index of highest valid vpage, map from virtual addresses to v_page structs                                                           
struct process{
  int pid;
  page_table_t ptable; // loop over every entry in array and set ppage field to unsigned int -1, RW fields to 0                                                                                             
  int curr_valid_page=0;
  map<int,vm_page*> v_pages;}; // vmfault passed in VA, determine if VA is valid. to check calculate page from address and see if that same address in map                                                  


//global vars                                                                                                                                                                                          
//queue for disk blocks, pop off when allocated to a particular virtual page, push back on when that process is destroyed                                                                              
queue<unsigned int> disk_blocks_counter;


//queue for free physical pages, pop off when allocated to a v_page, push back on when that process is destroyed                                                                                       
queue<unsigned long> phys_free;

//queue for the clock, implement in helper function later                                                                                                                                              
queue<unsigned int> clock_queue;

//current process, used when switching processes                                                                                                                                                       
process* curr_process;

//map of process ids to process struct pointers                                                                                                                                                        
map<int,process*> all_processes;

//start all ptes with read and write set to 0, on first access, set referenced bit? need set to 0 kinda always right?                                                                                  


// Lesley: I marked it to void                                                                                                                                                                              
void vm_init(unsigned int memory_pages, unsigned int disk_blocks){
  //needs to run when pager starts, set global vars (maybe using the sizes and things in the header file)                                                                                              

  // append to phys_free queue                                                                                                                                                                              
  // append to disk_blocks queue                                                                                                                                                                            
  for (unsigned int i = 0; i < memory_pages; i++) {
    phys_free.push(i);
  }
  for (unsigned int i = 0; i < disk_blocks; i++) {
    disk_blocks_counter.push(i);
  }
  cout << "pager initialized with " << memory_pages << " physical pages and " << disk_blocks << " disk blocks.\n";
}

void vm_create(pid_t pid){
  //needs to run when a new process is created,                                                                                                                                                        

  //sets all the stuff each process needs (all the things in the process struct)                                                                                                                       

  //initialize all ptes in the hardware page table with -1 for phys address and 00 for read and write                                                                                                  

  //add process to the all_processes global                                                                                                                                                                 

  process* p = new process;
  p->pid = pid;
  page_table_t* temp = new page_table_t;
  p->ptable = *temp;

  for (unsigned int i = 0; i < VM_ARENA_SIZE / VM_PAGESIZE; i++) {
    p->ptable.ptes[i].ppage = -1;
    p->ptable.ptes[i].read_enable = 0;
    p->ptable.ptes[i].write_enable = 0;
  }

  p->curr_valid_page = -1;
  all_processes[pid] = p;
  cout << "created process " << pid << endl;
 
}


void vm_switch(pid_t pid){
  curr_process = all_processes[pid];
  page_table_base_register = &(curr_process->ptable);
}
//switches to a new process (just pointing to new arena of memory, update the curr_process global)                                                                                                     

//helper function for gettnig new physical page, will implement clock here later                                                                                                                            
unsigned long get_new_ppage(){
  if(phys_free.empty()){
      cout<<"no more free pages. time to evict!"<<endl;
      return -1;}
  unsigned long ppage = phys_free.front();
  phys_free.pop();
  return ppage;}

int vm_fault(void *addr, bool write_flag){

 //cases:                                                                                                                                                                                                   
  //1. is the address valid. No? return -1.                                                                                                                                                            

  //2. is page resident                                                                                                                                                                                

  //    if not, get free page (ues helper function, may need to run the clock), update page table, update read and write bits based on type of fault                                                   

  //         is virtual page on disk (not zero)? If so, populate page from disk. Else, zero fill the page.                                                                                             

  //    if yes,                                                                                                                                                                                        

  //        read fault? if dirty, set r/w to 1.(the clock had changed permissions) if clean, set r=1.                                                                                                  

  //        write fault? set r/w = 1 //set dirty bit on all write faults                                                                                                                               

  //        clock must have run, set resident                                                                                                                                                               


  // compute virtual page number            
  uintptr_t address = (uintptr_t) addr;
  int vpage = (address - (uintptr_t)VM_ARENA_BASEADDR) / VM_PAGESIZE;
  //1. is the address valid. No? return -1.                                                                                                                                                                 
  if ((all_processes.find(curr_process->pid)  == all_processes.end()) || curr_process->v_pages.find(vpage) == curr_process->v_pages.end()) {
    return -1;
  }

  vm_page* page = curr_process->v_pages[vpage];
  page_table_entry_t* pte = &(curr_process->ptable.ptes[vpage]);

  //2. check if ppage is resident                                                                                                                                                                           
  if (pte->ppage == -1) {
    // if not, get free page                                                                                                                                                                                
    unsigned long ppage = get_new_ppage();
    pte->ppage = ppage;
    page->reference = 1;
    //was is a write fault?                                                                                                                                                                                 
    if (write_flag == 1) {
      pte->read_enable = 1;
      pte->write_enable = 1;
      page->dirty = 1; }
    else {//then must be read fault                                                                                                                                                                          
      pte->read_enable = 1; }
    //if on disk, read in                                                                                                                                                                                   
    //    if (page->zero == false) {
    // disk_read(page->disk_location, ppage); }
    //if not, zero fill page                                                                                                                                                                                
    if(page->zero==true) {
      memset(&((char *)pm_physmem)[ppage * VM_PAGESIZE], 0, VM_PAGESIZE); }
    //      for(int i=0;i<VM_PAGESIZE;i++){                                                                                                                                                               
    //not totally sure this works... need to check                                                                                                                                                      
    //((char *)pm_physmem)[vpage+i]=0;}}                                                                                                                                                                
  }
  //else if resident                        
  else{
    if(write_flag==1){
      pte->read_enable=1;
      pte->write_enable = 1;
      page->dirty=1;}
    else{
      pte->read_enable =1;
      if(page->dirty==1){
        pte->write_enable=1;}}
    page->reference=1;}

  return 0;}



void vm_destroy(){
  //push disk blocks for all virtual pages of process back on disk block queue                                                                                                                         

  //push phys pages for all virual pages of process back on phys page queue                                                                                                                            

  //remove process from all_processes                                                                                                                                                                  

  //delete anything that got a new                                                                                                                                                                     

  //probably other things                                                                                                                                                                              

  //  return 0;}                         
process* p = curr_process;
for(int i=0;i<=p->curr_valid_page;i++){
  if (p->v_pages.count(i)) {
    disk_blocks_counter.push(p->v_pages[i]->disk_location);
    delete p->v_pages[i];
  }
}

  for(int i=0;i<=p->curr_valid_page;i++){
    if (p-> ptable.ptes[i].ppage != 129) {//confused about this line
        phys_free.push(p->ptable.ptes[i].ppage);}
    }

  if (all_processes.count(p->pid)) {
    delete all_processes[p->pid];
    all_processes.erase(p->pid);
  }
  
  cout << "destroyed process" << p->pid << endl;
}


void * vm_extend(){// do NOT touch any ppages                                                                                                                                                               

  //create a new virtual page object, add to process's v_pages, update curr_valid_page                                                                                                                 

  // i know how many pages i have, so multiply page number by page size to get VA                                                                                                                           
  // base page is base adder (0x00060) divided by page size                                                                                                                                                 
  // return lowest address                                                                                                                                                                                  

  //check if we exceed the limit/if we have a free disk                                                                                                                                                     
  cout<<"in extend"<<endl;
  if (curr_process->curr_valid_page + 1 > VM_ARENA_SIZE / VM_PAGESIZE) {
    return nullptr;
  }
  if (disk_blocks_counter.empty()) {
    return nullptr;
  }

 // make new virtual page at new_vpage index in page table                                                                                                                                                 
  /// give it a disk block :)                                                                                                                                                                               
  int new_vpage = curr_process->curr_valid_page + 1;
  vm_page* new_page = new vm_page();
  new_page->disk_location = disk_blocks_counter.front(); //check this                                                                                                                                       
  disk_blocks_counter.pop();

  // add to the process struct map                                                                                                                                                                          
  curr_process->v_pages[new_vpage] = new_page;
  curr_process->curr_valid_page = new_vpage;  // little confused about the function of curr_valid_page field                                                                                                 

  //virtual address = page number * page size + base???
  
  //  void* vaddr = (void*) *(VM_ARENA_BASEADDR) + new_vpage * VM_PAGESIZE;
  unsigned int temp =  (0x60000000 + new_vpage * VM_PAGESIZE);
  void* vaddr = (void*)(&temp);
  //  unsigned int vaddr = (unsigned int)(VM_ARENA_BASEADDR+(new_vpage*VM_PAGESIZE));
  return vaddr;

}

int vm_syslog(void *message, unsigned int len){
  return 0;
}
