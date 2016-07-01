#!/usr/bin/env python

# judge.py
# APTO2016

from sys import *


viewer = False





##########################################
#
# funkcje pomocnicze


def error( s ):
  print "BLAD: %s" % s 
  exit(-1)



##########################################
#
# wczytywanie planszy



# wczytaj plansze
# zwraca (N,K, plansza)
def readBoard( f ):
  try:
    N = int( f.readline() )
    K = int( f.readline() )
    board = []
    for i in range(N):
      board += [ [ int(x) for x in f.readline().split()] ]
      
    return (N, K, board)

  except: 
   error( "Blad podczas wczytywania planszy" )  


  if( viewer ):
    printBoard( board )



# wczytaj rozwiazanie
# zwraca (N,K, plansza)
def readSolution( f, N ):
  try:
    solution = []
    for line in f:
      move = [ int(x) for x in line.split()]
      if( len(move) == 0 ):
        continue
      for i in range(4):
        if( move[i] < 0 or move[i] >= N ):
          error( "Bledny format rozwiazania")
      solution += [move]
 
    return solution

  except: 
   error( "Blad podczas wczytywania rozwiazania" )  






  
  
##########################################
#
# wypisywanie planszy

def printBoard( board ):
  M = max( [max(row) for row in board])
  L = len(str(M))+1
  for row in board:
    r = ""
    for cell in row:
      x = str(cell)
      r += " "*(L-len(x))+x
    print r
  print ""




##########################################
#
# weryfikacja rozwiazania


def checkMove( sr,sc, tr,tc, board ):
  dr = tr-sr
  lr = abs( dr )
  dc = tc-sc
  lc = abs( dc )

  if( dr < 0 ): dr = -1
  if( dr > 0 ): dr = 1
  if( dc < 0 ): dc = -1
  if( dc > 0 ): dc = 1

  if( lr != 0 and lc != 0):
    if( lr != lc ):
      error("ruch (%d,%d) --> (%d,%d) jest niedozwolony" % (sr,sc,tr,tc) )

  ar = sr+dr
  ac = sc+dc
  for t in range(max(lr,lc)-1):
    if( board[ar][ac] != 0 ):
      error("ruch (%d,%d) --> (%d,%d) jest niemozliwy; blokada przez innego hetmana" % (sr,sc,tr,tc) )
    ar += dr
    ac += dc




def executeSolution( board, solution ):
  for move in solution:
    (sr,sc,tr,tc) = move
    source = board[sr][sc]
    target = board[tr][tc]
    if( source == 0 or target == 0):
      error( "Ruch (%d,%d) --> (%d,%d) niemozliwy: brak hetmana" % (sr,sc,tr,tc) )
    if( source != target ):
      error( "Ruch (%d,%d) --> (%d,%d) niemozliwy: rozne wartosci hetmanow" % (sr,sc,tr,tc) )
    checkMove( sr,sc, tr,tc, board )
    board[sr][sc] = 0
    board[tr][tc] = 2*target



##########################################
#
# policz hetmanow

def countQueens( board ):
  return sum( [ sum( [ min(x,1) for x in row ] )  for row in board ] )







##########################################
#
# program glowny



def main():
  if( len(argv) != 3 ):
    error("Wywolanie: judge.py input output")

  try:
    f = open( argv[1], "r" )
    (N, K, board) = readBoard( f )
  except:
    error("Nie udalo sie wczytac planszy z opisem problemu")

  try:
    f = open( argv[2], "r" )
    solution = readSolution( f, N )
  except:
    error("Nie udalo sie wczytac rozwiazania")

  # wykonaj rozwiazanie
  if( viewer ): printBoard( board ) 
  executeSolution( board, solution )
  if( viewer ): printBoard( board ) 
  

  # oswietlenie obliczone, pora narysowac plansze
  if( countQueens( board ) <= K ):
    print "OK"
    exit(0)
  else: 
    print "Za duzo hetmanow"
    exit(-1)



try:
  main()
except Exception:
  error("Nieznany blad sedziego")


