#include "vm_pager.h"
#include <iostream>
#include <algorithm>
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
  int owner_pid;
};

//addresses are from the starting place in mem, find pte number by dividing by page size                                                                                                            
//struct for each process, keeps process id, hardware page table, index of highest valid vpage, map from virtual addresses to v_page structs                                                           
struct process{
  int pid;
  page_table_t ptable;                                                                                         
  unsigned int curr_vpage=0;
  uintptr_t curr_addr=(uintptr_t)VM_ARENA_BASEADDR;
  map<int,vm_page*> v_pages;};                                               


//GLOBAL VARIABLES                                                                                                                                                                                         
//queue for disk blocks, pop off when allocated to a particular virtual page, push back on when that process is destroyed                                                                              
queue<unsigned int> disk_blocks_counter;

//queue for free physical pages, pop off when allocated to a v_page, push back on when that process is destroyed                                                                                       
queue<unsigned long> phys_free;

//queue for the clock, implement in helper function later                                                                                                                                              
queue<vm_page*> clock_queue;

//current process, used when switching processes                                                                                                                                                       
process* curr_process;

//map of process ids to process struct pointers                                                                                                                                                        
map<int,process*> all_processes;


void vm_init(unsigned int memory_pages, unsigned int disk_blocks){                                                                                                                                                                           
  for (unsigned int i = 0; i < memory_pages; i++) {
    phys_free.push(i);
  }
  for (unsigned int i = 0; i < disk_blocks; i++) {
    disk_blocks_counter.push(i);
  }
}

void vm_create(pid_t pid){
  //needs to run when a new process is created,                                                                                                                                                        
  //sets all the stuff each process needs (all the things in the process struct)                                                                                                                       
  //initialize all ptes in the hardware page table with -1 for phys address and 00 for read and write                                                                                                                                                                                                                                                                

  process* p = new process;
  p->pid = pid;

  for (unsigned int i = 0; i < VM_ARENA_SIZE / VM_PAGESIZE; i++) {
    p->ptable.ptes[i].ppage = 129;
    p->ptable.ptes[i].read_enable = 0;
    p->ptable.ptes[i].write_enable = 0;
  }

  all_processes[pid] = p; 
}

//switches to a new process (just pointing to new arena of memory, update the curr_process global)                                                                                                     
void vm_switch(pid_t pid){
  curr_process = all_processes[pid];
  page_table_base_register = &(curr_process->ptable);
}

//helper function for gettnig new physical page
unsigned long run_clock(){
  if(clock_queue.empty()){
    return -1;
  }
  bool found = false;
  while(found == false){
    vm_page* temp = clock_queue.front();
    clock_queue.pop();
    // Find the process that owns the virtual page
    process* owner = all_processes[temp->owner_pid];
    page_table_entry_t* temp_pte = &(owner->ptable.ptes[temp->pte]);
    if(temp->reference==1){
      //if referenced, set ref bit, read and write permissions so that next reference will fault, push to back of queue
      temp->reference = 0;
      temp_pte->read_enable=0;
      temp_pte->write_enable=0;
      clock_queue.push(temp);
    }
    else {
      if(temp->dirty==1){
	      disk_write(temp->disk_location,temp_pte->ppage);
        temp->dirty=0;
        temp->zero = false;
      }
      found = true;
      unsigned long val = temp_pte->ppage;
      //set resident to false
      temp_pte->ppage=129;
      temp_pte->read_enable=0;
      temp_pte->write_enable=0;
      return val;
      
    }
  }
  return -1;
}

unsigned long get_new_ppage(vm_page* v_page){
  unsigned long ppage;
  if(phys_free.empty()){
    ppage = run_clock();
  }
  else{//if there are still unused ppages...
    ppage = phys_free.front();
    phys_free.pop();
    //first time that page is assigned, need to add to the clock queue
  }
  return ppage;
}


  
int vm_fault(void *addr, bool write_flag){  
  //cases for vm_fault:                                                                                                                                                                                                   
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
  // is the address valid. No? return -1.                                                                                                                                                                 
  if ((all_processes.find(curr_process->pid)  == all_processes.end()) || curr_process->v_pages.find(vpage) == curr_process->v_pages.end()) {
    return -1;
  }

  vm_page* page = curr_process->v_pages[vpage];
  page_table_entry_t* pte = &(curr_process->ptable.ptes[vpage]);

  // Case 1: not resident                                                                                                                                                                          
  if (pte->ppage == 129) {
    // if not, get free page                                                                                                                                                                                
    unsigned long ppage = get_new_ppage(page);
    pte->ppage = ppage;
    //was it a write fault?                                                                                                                                                                                 
    if (write_flag == 1) {
      pte->read_enable = 1;
      pte->write_enable = 1;
      page->dirty = 1; }
    else {//then must be read fault                                                                                                                                                                          
      pte->read_enable = 1;
      pte -> write_enable = 0; }
    //if on disk, read in                                                                                                                                                                                   
    //if not, zero fill page                                                                                                                                                                                
    if(page->zero==true) {
      memset(&((char *)pm_physmem)[ppage * VM_PAGESIZE], 0, VM_PAGESIZE); 
    } 
    else {
      disk_read(page->disk_location, ppage);
    }
    page->reference = 1;
    clock_queue.push(page);

  }
  //else if resident                        
  else{
    if(write_flag){
      pte->read_enable=1;
      pte->write_enable = 1;
      page->dirty=1;
    }
    else{
      pte->read_enable =1;
      pte->write_enable=0;
      if(page->dirty==1){
        pte->write_enable=1;
      }
    }
    page->reference=1;
  }

  return 0;}


void vm_destroy(){                        
  process* p = curr_process;
  if (p == nullptr) return;

  // remove pages from the clock queue
  queue<vm_page*> new_clock;
    while (!clock_queue.empty()) {
        vm_page* vpage = clock_queue.front();
        clock_queue.pop();
        if (vpage->owner_pid != p->pid) {
            new_clock.push(vpage);
        }
    }
    clock_queue = new_clock;

  // Free the virtual pages and physical pages
  for (auto& entry : p->v_pages) {
    vm_page* vpage = entry.second;
    page_table_entry_t* pte = &(p->ptable.ptes[entry.first]);

    //push disk blocks for all virtual pages of process back on disk block queue 
    disk_blocks_counter.push(vpage->disk_location);

    // if resident, return ppage to free list
    if (pte->ppage != 129) {
      phys_free.push(pte->ppage);
    }

    delete vpage;
  }
  p->v_pages.clear();  

  //remove process from all_processes   
  all_processes.erase(p->pid);
  delete p;
  curr_process = nullptr;
}


void * vm_extend(){                                                                                                                                                            
  //create a new virtual page object, add to process's v_pages, update curr_valid_page                                                                                                                                                                                                                                                                                     
  if (curr_process->curr_vpage + 1 > VM_ARENA_SIZE / VM_PAGESIZE) {
    return nullptr;
  }
  if (disk_blocks_counter.empty()) {
    return nullptr;
  }

  // make new virtual page at new_vpage index in page table                                                                                                                                                 
  // give it a disk block :)                                                                                                                                                                               
  int vpage = curr_process->curr_vpage;
  vm_page* new_page = new vm_page();
  new_page->owner_pid = curr_process->pid;
  new_page->disk_location = disk_blocks_counter.front();                                                                                                                                     
  disk_blocks_counter.pop();
  new_page->pte = vpage;
  curr_process->ptable.ptes[vpage].ppage = 129;

  // add to the process struct map                                                                                                                                                                          
  curr_process->v_pages[vpage] = new_page;

  void* vaddr = (void*)(curr_process->curr_addr);
   curr_process->curr_vpage = vpage+1;
   curr_process->curr_addr = curr_process->curr_addr+VM_PAGESIZE;
  return vaddr;

}

int vm_syslog(void *message, unsigned int len){
  string ans;
  uintptr_t start = (uintptr_t)message;
  unsigned int vpage = ((uintptr_t)(start))/VM_PAGESIZE-VM_ARENA_BASEPAGE;
  page_table_entry_t* vpage_ptr = &(curr_process ->ptable.ptes[vpage]);
  unsigned int offset = (uintptr_t)start % VM_PAGESIZE;
  unsigned int size = min(((unsigned int)VM_PAGESIZE)-offset,len);

  if((unsigned int)start + len<(unsigned int)start){
    return -1;}
  if(len ==0){
    return -1;}
  while(len>0){
    //if not resident
    if(vpage>=curr_process->curr_vpage){
    return -1;}
    if(vpage_ptr->ppage ==129||vpage_ptr->read_enable==0){
      vm_fault((void*)start,false);}
    ans.append((char*) pm_physmem+vpage_ptr->ppage*VM_PAGESIZE+offset,size);
    vpage = vpage+1;
    vpage_ptr = &(curr_process ->ptable.ptes[vpage]);
    len = len-size;
    start += size;
    size = min((unsigned int)VM_PAGESIZE,len);
    offset = 0;
	       
  }
  cout << "syslog \t\t\t" << ans << endl;
  return 0;
}

